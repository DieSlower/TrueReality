/*
* True Reality Open Source Game and Simulation Engine
* Copyright � 2018 Acid Rain Studios LLC
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 3.0 of the License, or (at your option)
* any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* @author Maxim Serebrennik
*/

#pragma once

#include <GL/glew.h> //Needs to be included before anything with gl.h

#include "Utils.h"
#include "TxtrCameraObject.h"
#include "WinDefaultConfig.h"

#include <trBase/Quat.h>
#include <trBase/SmrtPtr.h>
#include <trBase/Vec3.h>
#include <trCore/SceneObjects/RingArray.h>
#include <trCore/SceneObjects/RingArrayCallback.h>
#include <trCore/SceneObjects/SkyBoxNode.h>
#include <trUtil/Console/Logo.h>
#include <trUtil/DefaultSettings.h>
#include <trUtil/PathUtils.h>
#include <trUtil/RefStr.h>

#include <osg/Camera>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/StandardManipulator>
#include <osgViewer/CompositeViewer>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>

#include <iostream>

static const trUtil::RefStr COW_MODEL = trUtil::RefStr(trUtil::PathUtils::GetStaticMeshesPath() + "/OsgCow/OsgCow.osgb");
static const trUtil::RefStr CUBE_MODEL = trUtil::RefStr(trUtil::PathUtils::GetStaticMeshesPath() + "/ColorCube/ColorCube.osgb");
static const trUtil::RefStr SKY_BOX_MODEL = trUtil::RefStr(trUtil::PathUtils::GetStaticMeshesPath() + "/ConstructSkybox/ConstructSkybox.osgb");

static const int WIN_WIDTH = 1280;
static const int WIN_HEIGHT = 720;
static const int WIN_POS_X = 100;
static const int WIN_POS_Y = 100;

static const double CAM_NEAR_CLIP = 0.1;
static const double CAM_FAR_CLIP = 1000000.0;
static const double CAM_FOV = 60;

static const int SAMPLE_NUM = 4;


/**
 * @fn  int main(int argc, char** argv)
 *
 * @brief   Main entry-point for this application.
 *
 * @param   argc    The number of command-line arguments provided.
 * @param   argv    An array of command-line argument strings.
 *
 * @return  Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char** argv)
{
    //Show Logo
    trUtil::Console::Logo();

    std::cout << "OSG Mpeg Demo\n\n" << std::endl;

    //Check for command line options.
    std::string mpegType = "";
    std::string fileName = "";
    std::string ip = "";
    std::string logFileName = "";
    std::string logLevel = "";
    ParseCmdLineArgs(argc, argv, mpegType, fileName, ip, logFileName, logLevel);

    //Creates the default folders in the User Data folder. 
    trUtil::PathUtils::CreateUserDataPathTree();

    //Setup our Logging options
    trUtil::DefaultSettings::SetupLoggingOptions(logFileName, logLevel);    

    //Create the root node
    trBase::SmrtPtr<osg::Group> rootNode = new osg::Group();

    //Create our scene
    trBase::SmrtPtr<trCore::SceneObjects::RingArray> ringNode = new trCore::SceneObjects::RingArray(80);
    trBase::SmrtPtr<trCore::SceneObjects::RingArrayCallback> modelCallback = new trCore::SceneObjects::RingArrayCallback();
    ringNode->setUpdateCallback(modelCallback);

    //Place a cube in the texture scene
    trBase::SmrtPtr<osg::PositionAttitudeTransform> cube = new osg::PositionAttitudeTransform();
    cube->addChild(osgDB::readNodeFile(CUBE_MODEL));
    cube->setScale(trBase::Vec3(400, 400, 400));
    cube->setAttitude(trBase::Quat(0.2, trBase::X_AXIS, 0.2, trBase::Y_AXIS, 0.2, trBase::Z_AXIS));
    ringNode->addChild(cube);

    //Place a light in the texture scene
    trBase::SmrtPtr<osg::LightSource> light = CreateLight(0);
    ringNode->addChild(light.Get());

    //Create the render to texture structure
    trBase::SmrtPtr<TxtrCameraObject> camObject = new TxtrCameraObject(ringNode, WIN_WIDTH, WIN_HEIGHT, mpegType, fileName, ip);

    //Create our View
    trBase::SmrtPtr<osgViewer::View> mainView = new osgViewer::View();
    mainView->setSceneData(rootNode.Get());
    mainView->apply(new WinDefaultConfig(WIN_POS_X, WIN_POS_Y, WIN_WIDTH, WIN_HEIGHT, 0U, true, CAM_NEAR_CLIP, CAM_FAR_CLIP, CAM_FOV));

    //Adds the statistics handler. 
    mainView->addEventHandler(new osgViewer::StatsHandler);    

    trBase::SmrtPtr<trCore::SceneObjects::SkyBoxNode> skyBox = new trCore::SceneObjects::SkyBoxNode();
    skyBox->LoadFile(SKY_BOX_MODEL);

    //Setup the scene
    rootNode->addChild(skyBox);
    rootNode->addChild(camObject->GetCamera());
    rootNode->addChild(camObject->GetRenderTarget());
    rootNode->addChild(osgDB::readNodeFile(COW_MODEL));

    //Set up the viewer
    osgViewer::CompositeViewer viewer;
    viewer.addView(mainView);
    
    //Set up the main frame loop
    if ((mainView->getCameraManipulator() == 0) && mainView->getCamera()->getAllowEventFocus())
    {
        mainView->setCameraManipulator(new osgGA::TrackballManipulator());
    }
    viewer.setReleaseContextAtEndOfFrameHint(true);
    if (!viewer.isRealized())
    {
        viewer.realize();
    }

    //Run the main frame loop
    while (!viewer.done())
    {
        viewer.frame();
    }
    return 0;
}