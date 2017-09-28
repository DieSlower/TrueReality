/*
* True Reality Open Source Game and Simulation Engine
* Copyright � 2017 Acid Rain Studios LLC
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
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
* Author: Maxim Serebrennik
*/

#include <trUtil/VersionUtil.h>

#include <trUtil/StringUtils.h>
#include <trUtil/PathUtils.h>
#include <trUtil/FileUtils.h>
#include <trUtil/DateTime.h>
#include <trUtil/Logging/Log.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <ctime>

namespace trUtil
{
    const std::string VersionUtil::VERSION_FILE_NAME = std::string("Version.tcver");
    const std::string VersionUtil::MAJ_VERSION = std::string("Major");
    const std::string VersionUtil::MIN_VERSION = std::string("Minor");
    const std::string VersionUtil::YYMM_VERSION = std::string("YYMM");
    const std::string VersionUtil::BUILD_VERSION = std::string("Build");
    
    //////////////////////////////////////////////////////////////////////////
    VersionUtil::VersionUtil()
    {
        mVersion.SetFileName(VERSION_FILE_NAME);
        mVersion.SetFilePath(trUtil::PathUtils::GetDataPath() + PathUtils::CONFIG_PATH);
        
        //If there is no version file, create one
        if (mVersion.FileExists())
        {
            mVersion.ReadFromFile(VERSION_FILE_NAME);
        }
        else
        {
            std::string errMsg = "Version File does not exist, generating a new one...";			
            LOG_W("Looking for file in : " + mVersion.GetFilePath())
            LOG_W(errMsg)
            std::cout << errMsg << std::endl;
            GenerateVersionStructure();
            SaveVersionFile();
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    VersionUtil::~VersionUtil()
    {
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SaveVersionFile()
    {
        mVersion.WriteToFile();
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetVersion(int maj, int min, std::string yymm, int build)
    {
        SetMajorVersion(maj);
        SetMinorVersion(min);
        SetYYMMVersion(yymm);
        SetBuildVersion(build);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetVersion(int maj, int min, int yymm, int build)
    {
        SetMajorVersion(maj);
        SetMinorVersion(min);
        SetYYMMVersion(yymm);
        SetBuildVersion(build);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::GenerateVersionStructure()
    {
        SetVersion(0, 0, GetTodaysVersionDate(), GetCurrentCommitNum());
        
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::IncrementVersion()
    {
        SetYYMMVersion(GetTodaysVersionDate());
        SetBuildVersion(GetBuildVersion()+1);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetMajorVersion(int maj)
    {
        mVersion.SetInt(MAJ_VERSION, maj);
    }
    
    //////////////////////////////////////////////////////////////////////////
    int VersionUtil::GetMajorVersion()
    {
        return mVersion.GetInt(MAJ_VERSION);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetMinorVersion(int min)
    {
        mVersion.SetInt(MIN_VERSION, min);
    }
    
    //////////////////////////////////////////////////////////////////////////
    int VersionUtil::GetMinorVersion()
    {
        return mVersion.GetInt(MIN_VERSION);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetYYMMVersion(std::string yymm)
    {
        mVersion.SetString(YYMM_VERSION, yymm);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetYYMMVersion(int yymm)
    {
        mVersion.SetInt(YYMM_VERSION, yymm);
    }
    
    //////////////////////////////////////////////////////////////////////////
    std::string VersionUtil::GetYYMMVersion()
    {
        return mVersion.GetString(YYMM_VERSION);
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::SetBuildVersion(int build)
    {
        mVersion.SetInt(BUILD_VERSION, build);
    }
    
    //////////////////////////////////////////////////////////////////////////
    int VersionUtil::GetBuildVersion()
    {
        return mVersion.GetInt(BUILD_VERSION);
    }
    //////////////////////////////////////////////////////////////////////////
    std::string VersionUtil::GetVersionString()
    {
        std::stringstream ver;
        
        ver << GetMajorVersion() << "." << GetMinorVersion() << "." << GetYYMMVersion() << "." << GetBuildVersion();
        
        if (GetMajorVersion() == 0 && GetMinorVersion() == 0)
        {
            LOG_E("Version should not be 0.0, something is wrong...")
        }
        return ver.str();
    }
    
    //////////////////////////////////////////////////////////////////////////
    std::string VersionUtil::GetTodaysVersionDate()
    {
        DateTime date(time(0)); //Gets todays date
        std::stringstream ver;
        std::string temp;
        
        //Convert the year to a string
        ver << date.GetYear();
        temp = ver.str();
        
        //Check if we have the full year format of 4 characters ex: "1999" and grab the last two
        if (temp.size() == 4)
        {
            ver.str(std::string()); //Clears the stream
            ver << temp[2] << temp[3];
        }
        else
        {
            //If the format is wrong, return nothing
            return "";
        }
        
        //Appends the month value, making the month always have 2 digits. Ex 6 would be 06
        ver << std::setfill('0') << std::setw(2) << date.GetMonth();
        
        return ver.str();
    }
    
    //////////////////////////////////////////////////////////////////////////
    int VersionUtil::GetCurrentCommitNum()
    {
        try
        {
            //Get the revision number string from the HG repo
            std::string rev = trUtil::FileUtils::GetInstance().RunCommand("hg -R " + PathUtils::GetRootPath() + " identify --num");
            
            if (rev == trUtil::StringUtils::STR_BLANK)
            {
                LOG_E("No .hg folder found at the TR_ROOT path. \nTR_ROOT: " + PathUtils::GetRootPath());
                std::cout << ".hg (HG Repo) is needed for this to work" << std::endl;
            }
            
            //Extract the revision number from the string
            trUtil::StringUtils::FindAndReplace(rev, "\n", "");
            trUtil::StringUtils::FindAndReplace(rev, "+", "");
            
            //Convert the string to an integer
            return trUtil::StringUtils::FromString<int>(rev);
        }
        catch (trUtil::Exception ex)
        {
            ex.LogException();
        }		
        return NULL;
    }
    
    //////////////////////////////////////////////////////////////////////////
    void VersionUtil::UpdateVersion()
    {
        //Adds +1 to the current commit number to encompass the version commit. 
        SetVersion(GetMajorVersion(), GetMinorVersion(), GetTodaysVersionDate(), GetCurrentCommitNum()+1);
    }
}