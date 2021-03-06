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

#include <Examples/MPEGServer/Utils.h>

#include <trUtil/ArgumentParser.h>
#include <trUtil/Exception.h>
#include <trUtil/Logging/Log.h>

#include <osg/Light>

#include <iostream>
#include <cstdlib>
#include <sstream>

void ParseCmdLineArgs(int& argc, char** argv, std::string& mpegType, std::string& fileName, std::string& ip, std::string& logFileName, std::string& logLevel)
{
    trUtil::ArgumentParser arguments(&argc, argv);

    arguments.SetApplicationName(PROGRAM_NAME);
        
    arguments.AddCommandLineOption("\n--help, /help, -h, /h, /?  ", "Show this help screen.");
    arguments.AddCommandLineOption("\n--logFileName <filename>   ", "The name of the log file to use.  Defaults to TrueRealityLog.html");
    arguments.AddCommandLineOption("\n--logLevel <level>         ", "Logging level to use. \nLevel options are: " + trUtil::Logging::LOG_DEBUG_STR + ", " +
        trUtil::Logging::LOG_INFO_STR + ", " +
        trUtil::Logging::LOG_WARNING_STR + ", " +
        trUtil::Logging::LOG_ERROR_STR + "");
    arguments.AddCommandLineOption("\n--ip <IP:PORT>", "Sets the IP and Port for broadcasting");
    arguments.AddCommandLineOption("\n--mpegType <Mpeg Type>", "Records a file with a given mpeg format. Values are mpeg2, mpeg4, h264, h265");
    arguments.AddCommandLineOption("\n--output <File Name/UDP>", "Sets a file name (Do not put an extension). Use UDP for the name to start a broadcast");

    arguments.AddCommandLineOption("Example: " + std::string(argv[0]) + " --mpegType h264", "Records OutputVid.h264");
    arguments.AddCommandLineOption("Example: " + std::string(argv[0]) + " --mpegType mpeg2 --output test", "Records test.mpg");
    arguments.AddCommandLineOption("Example: " + std::string(argv[0]) + " --mpegType mpeg4 --output MyTest", "Records MyTest.mp4");
    arguments.AddCommandLineOption("Example: " + std::string(argv[0]) + " --mpegType h265 --output UDP --ip 130.46.208.38:7000", "Broadcasts h265 format through UDP on 130.46.208.38:7000");
              
    if (arguments.Read("--help") == true ||
        arguments.Read("/help") == true ||
        arguments.Read("-h") == true ||
        arguments.Read("/h") == true ||
        arguments.Read("/?") == true)
    {
        arguments.Write(std::cout);
        exit(0);
    }

    arguments.Read("--logFileName", logFileName);
    arguments.Read("--logLevel", logLevel);
    arguments.Read("--mpegType", mpegType);
    arguments.Read("--output", fileName);
    arguments.Read("--ip", ip);

    //Check what kind of MPEG compression we want. 
    if ((mpegType != "mpeg4") && (mpegType != "h264") && (mpegType != "h265"))
    {
        mpegType = "mpeg2";
    }

    //Check if we have a set IP if UDP was selected
    if ((fileName == "UDP") && (ip == ""))
    {
        fileName = "";
    }
}

//////////////////////////////////////////////////////////////////////////
osg::LightSource* CreateLight(int lightNum)
{
    trBase::SmrtPtr<osg::Light> light = new osg::Light;
    light->setLightNum(lightNum);
    light->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    light->setPosition(osg::Vec4(0.0f, 100.0f, 0.0f, 1.0f));
    light->setConstantAttenuation(0.00001f);

    osg::LightSource* lightSource = new osg::LightSource;
    lightSource->setLight(light);

    return lightSource;
}
