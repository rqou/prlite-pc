//#line 2 "/opt/ros/electric/stacks/driver_common/dynamic_reconfigure/templates/ConfigType.h"
// *********************************************************
// 
// File autogenerated for the pr2lite_chess package 
// by the dynamic_reconfigure package.
// Please do not edit.
// 
// ********************************************************/

/***********************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ***********************************************************/

// Author: Blaise Gassend


#ifndef __pr2lite_chess__LOCATORCONFIG_H__
#define __pr2lite_chess__LOCATORCONFIG_H__

#include <dynamic_reconfigure/config_tools.h>
#include <limits>
#include <ros/node_handle.h>
#include <dynamic_reconfigure/ConfigDescription.h>
#include <dynamic_reconfigure/ParamDescription.h>
#include <dynamic_reconfigure/config_init_mutex.h>

namespace pr2lite_chess
{
  class LocatorConfigStatics;
  
  class LocatorConfig
  {
  public:
    class AbstractParamDescription : public dynamic_reconfigure::ParamDescription
    {
    public:
      AbstractParamDescription(std::string n, std::string t, uint32_t l, 
          std::string d, std::string e)
      {
        name = n;
        type = t;
        level = l;
        description = d;
        edit_method = e;
      }
      
      virtual void clamp(LocatorConfig &config, const LocatorConfig &max, const LocatorConfig &min) const = 0;
      virtual void calcLevel(uint32_t &level, const LocatorConfig &config1, const LocatorConfig &config2) const = 0;
      virtual void fromServer(const ros::NodeHandle &nh, LocatorConfig &config) const = 0;
      virtual void toServer(const ros::NodeHandle &nh, const LocatorConfig &config) const = 0;
      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, LocatorConfig &config) const = 0;
      virtual void toMessage(dynamic_reconfigure::Config &msg, const LocatorConfig &config) const = 0;
    };

    typedef boost::shared_ptr<AbstractParamDescription> AbstractParamDescriptionPtr;
    typedef boost::shared_ptr<const AbstractParamDescription> AbstractParamDescriptionConstPtr;
    
    template <class T>
    class ParamDescription : public AbstractParamDescription
    {
    public:
      ParamDescription(std::string name, std::string type, uint32_t level, 
          std::string description, std::string edit_method, T LocatorConfig::* f) :
        AbstractParamDescription(name, type, level, description, edit_method),
        field(f)
      {}

      T (LocatorConfig::* field);

      virtual void clamp(LocatorConfig &config, const LocatorConfig &max, const LocatorConfig &min) const
      {
        if (config.*field > max.*field)
          config.*field = max.*field;
        
        if (config.*field < min.*field)
          config.*field = min.*field;
      }

      virtual void calcLevel(uint32_t &comb_level, const LocatorConfig &config1, const LocatorConfig &config2) const
      {
        if (config1.*field != config2.*field)
          comb_level |= level;
      }

      virtual void fromServer(const ros::NodeHandle &nh, LocatorConfig &config) const
      {
        nh.getParam(name, config.*field);
      }

      virtual void toServer(const ros::NodeHandle &nh, const LocatorConfig &config) const
      {
        nh.setParam(name, config.*field);
      }

      virtual bool fromMessage(const dynamic_reconfigure::Config &msg, LocatorConfig &config) const
      {
        return dynamic_reconfigure::ConfigTools::getParameter(msg, name, config.*field);
      }

      virtual void toMessage(dynamic_reconfigure::Config &msg, const LocatorConfig &config) const
      {
        dynamic_reconfigure::ConfigTools::appendParameter(msg, name, config.*field);
      }
    };

//#line 13 "../cfg/Locator.cfg"
      int h_rho;
//#line 14 "../cfg/Locator.cfg"
      int h_threshold;
//#line 15 "../cfg/Locator.cfg"
      int h_min_length;
//#line 16 "../cfg/Locator.cfg"
      int channel;
//#line 17 "../cfg/Locator.cfg"
      bool output_image;
//#line 138 "/opt/ros/electric/stacks/driver_common/dynamic_reconfigure/templates/ConfigType.h"

    bool __fromMessage__(dynamic_reconfigure::Config &msg)
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      int count = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        if ((*i)->fromMessage(msg, *this))
          count++;
      if (count != dynamic_reconfigure::ConfigTools::size(msg))
      {
        ROS_ERROR("LocatorConfig::__fromMessage__ called with an unexpected parameter.");
        ROS_ERROR("Booleans:");
        for (unsigned int i = 0; i < msg.bools.size(); i++)
          ROS_ERROR("  %s", msg.bools[i].name.c_str());
        ROS_ERROR("Integers:");
        for (unsigned int i = 0; i < msg.ints.size(); i++)
          ROS_ERROR("  %s", msg.ints[i].name.c_str());
        ROS_ERROR("Doubles:");
        for (unsigned int i = 0; i < msg.doubles.size(); i++)
          ROS_ERROR("  %s", msg.doubles[i].name.c_str());
        ROS_ERROR("Strings:");
        for (unsigned int i = 0; i < msg.strs.size(); i++)
          ROS_ERROR("  %s", msg.strs[i].name.c_str());
        // @todo Check that there are no duplicates. Make this error more
        // explicit.
        return false;
      }
      return true;
    }

    // This version of __toMessage__ is used during initialization of
    // statics when __getParamDescriptions__ can't be called yet.
    void __toMessage__(dynamic_reconfigure::Config &msg, const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__) const
    {
      dynamic_reconfigure::ConfigTools::clear(msg);
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        (*i)->toMessage(msg, *this);
    }
    
    void __toMessage__(dynamic_reconfigure::Config &msg) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      __toMessage__(msg, __param_descriptions__);
    }
    
    void __toServer__(const ros::NodeHandle &nh) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        (*i)->toServer(nh, *this);
    }

    void __fromServer__(const ros::NodeHandle &nh)
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        (*i)->fromServer(nh, *this);
    }

    void __clamp__()
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      const LocatorConfig &__max__ = __getMax__();
      const LocatorConfig &__min__ = __getMin__();
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        (*i)->clamp(*this, __max__, __min__);
    }

    uint32_t __level__(const LocatorConfig &config) const
    {
      const std::vector<AbstractParamDescriptionConstPtr> &__param_descriptions__ = __getParamDescriptions__();
      uint32_t level = 0;
      for (std::vector<AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        (*i)->calcLevel(level, config, *this);
      return level;
    }
    
    static const dynamic_reconfigure::ConfigDescription &__getDescriptionMessage__();
    static const LocatorConfig &__getDefault__();
    static const LocatorConfig &__getMax__();
    static const LocatorConfig &__getMin__();
    static const std::vector<AbstractParamDescriptionConstPtr> &__getParamDescriptions__();
    
  private:
    static const LocatorConfigStatics *__get_statics__();
  };
  
  template <> // Max and min are ignored for strings.
  inline void LocatorConfig::ParamDescription<std::string>::clamp(LocatorConfig &config, const LocatorConfig &max, const LocatorConfig &min) const
  {
    return;
  }

  class LocatorConfigStatics
  {
    friend class LocatorConfig;
    
    LocatorConfigStatics()
    {
//#line 13 "../cfg/Locator.cfg"
      __min__.h_rho = 1;
//#line 13 "../cfg/Locator.cfg"
      __max__.h_rho = 5;
//#line 13 "../cfg/Locator.cfg"
      __default__.h_rho = 1;
//#line 13 "../cfg/Locator.cfg"
      __param_descriptions__.push_back(LocatorConfig::AbstractParamDescriptionConstPtr(new LocatorConfig::ParamDescription<int>("h_rho", "int", 0, "The rho value for Hough transformation.", "", &LocatorConfig::h_rho)));
//#line 14 "../cfg/Locator.cfg"
      __min__.h_threshold = 0;
//#line 14 "../cfg/Locator.cfg"
      __max__.h_threshold = 255;
//#line 14 "../cfg/Locator.cfg"
      __default__.h_threshold = 50;
//#line 14 "../cfg/Locator.cfg"
      __param_descriptions__.push_back(LocatorConfig::AbstractParamDescriptionConstPtr(new LocatorConfig::ParamDescription<int>("h_threshold", "int", 0, "Threshold to use for Hough transformation.", "", &LocatorConfig::h_threshold)));
//#line 15 "../cfg/Locator.cfg"
      __min__.h_min_length = 1;
//#line 15 "../cfg/Locator.cfg"
      __max__.h_min_length = 640;
//#line 15 "../cfg/Locator.cfg"
      __default__.h_min_length = 100;
//#line 15 "../cfg/Locator.cfg"
      __param_descriptions__.push_back(LocatorConfig::AbstractParamDescriptionConstPtr(new LocatorConfig::ParamDescription<int>("h_min_length", "int", 0, "Minimum length of a line for Hough transformation.", "", &LocatorConfig::h_min_length)));
//#line 16 "../cfg/Locator.cfg"
      __min__.channel = 0;
//#line 16 "../cfg/Locator.cfg"
      __max__.channel = 2;
//#line 16 "../cfg/Locator.cfg"
      __default__.channel = 0;
//#line 16 "../cfg/Locator.cfg"
      __param_descriptions__.push_back(LocatorConfig::AbstractParamDescriptionConstPtr(new LocatorConfig::ParamDescription<int>("channel", "int", 0, "Channel to use for conversion to grayscale.", "", &LocatorConfig::channel)));
//#line 17 "../cfg/Locator.cfg"
      __min__.output_image = 0;
//#line 17 "../cfg/Locator.cfg"
      __max__.output_image = 1;
//#line 17 "../cfg/Locator.cfg"
      __default__.output_image = 1;
//#line 17 "../cfg/Locator.cfg"
      __param_descriptions__.push_back(LocatorConfig::AbstractParamDescriptionConstPtr(new LocatorConfig::ParamDescription<bool>("output_image", "bool", 0, "Turn on this setting to output the grayscale image with lines and intersections super-imposed.", "", &LocatorConfig::output_image)));
//#line 239 "/opt/ros/electric/stacks/driver_common/dynamic_reconfigure/templates/ConfigType.h"
    
      for (std::vector<LocatorConfig::AbstractParamDescriptionConstPtr>::const_iterator i = __param_descriptions__.begin(); i != __param_descriptions__.end(); i++)
        __description_message__.parameters.push_back(**i);
      __max__.__toMessage__(__description_message__.max, __param_descriptions__); 
      __min__.__toMessage__(__description_message__.min, __param_descriptions__); 
      __default__.__toMessage__(__description_message__.dflt, __param_descriptions__); 
    }
    std::vector<LocatorConfig::AbstractParamDescriptionConstPtr> __param_descriptions__;
    LocatorConfig __max__;
    LocatorConfig __min__;
    LocatorConfig __default__;
    dynamic_reconfigure::ConfigDescription __description_message__;
    static const LocatorConfigStatics *get_instance()
    {
      // Split this off in a separate function because I know that
      // instance will get initialized the first time get_instance is
      // called, and I am guaranteeing that get_instance gets called at
      // most once.
      static LocatorConfigStatics instance;
      return &instance;
    }
  };

  inline const dynamic_reconfigure::ConfigDescription &LocatorConfig::__getDescriptionMessage__() 
  {
    return __get_statics__()->__description_message__;
  }

  inline const LocatorConfig &LocatorConfig::__getDefault__()
  {
    return __get_statics__()->__default__;
  }
  
  inline const LocatorConfig &LocatorConfig::__getMax__()
  {
    return __get_statics__()->__max__;
  }
  
  inline const LocatorConfig &LocatorConfig::__getMin__()
  {
    return __get_statics__()->__min__;
  }
  
  inline const std::vector<LocatorConfig::AbstractParamDescriptionConstPtr> &LocatorConfig::__getParamDescriptions__()
  {
    return __get_statics__()->__param_descriptions__;
  }

  inline const LocatorConfigStatics *LocatorConfig::__get_statics__()
  {
    const static LocatorConfigStatics *statics;
  
    if (statics) // Common case
      return statics;

    boost::mutex::scoped_lock lock(dynamic_reconfigure::__init_mutex__);

    if (statics) // In case we lost a race.
      return statics;

    statics = LocatorConfigStatics::get_instance();
    
    return statics;
  }


}

#endif // __LOCATORRECONFIGURATOR_H__
