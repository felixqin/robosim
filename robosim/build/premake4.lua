local ode_lib = "ode_singled"

local plugins = {
  "field",
  "team",
  "rgcp",
}

--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------

  solution "robosim"

    language "C++"
    location (_ACTION)
    configurations {"Debug", "Release"}
    
    configuration "Debug"
      defines {"_DEBUG"}
      flags { "Symbols" }

    configuration "Release"
      flags { "Optimize" }

--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------

  project "robosim"

    kind "WindowedApp"
    location (_ACTION)
    targetdir ( "../devel/".._ACTION )

    includedirs {
        "../sdk/include",
        "../robosim/include"
    }
    libdirs {
        "../devel/".._ACTION
    }
    links { "glfw", "opengl32", "glu32", ode_lib, "sdk" }

    files { "../robosim/src/**.cpp" }
    if ( os.is("windows") ) then
      excludes { "../robosim/src/platform/linux/**.cpp" }
    end
    

--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------

  project "sdk"

    kind "SharedLib"
    location (_ACTION)
    defines { "SDK_EXPORTS" }
    targetdir ( "../devel/".._ACTION )
    targetname "robosim"

    includedirs {
        "../sdk/include"
    }
    links { ode_lib }

    files { "../sdk/src/**.cpp" }

--------------------------------------------------------------------------------
-- plugins
--------------------------------------------------------------------------------

  for _, name in ipairs(plugins) do
  
    project ("plugin_" .. name)

    kind "SharedLib"
    location (_ACTION)
    defines {"PLUGIN_EXPORTS"}
    targetdir ( "../devel/".._ACTION.."/plugins" )
    targetname (name)

    includedirs {
        "../sdk/include"
    }
    links { ode_lib, "sdk" }

    files { "../plugins/"..name.."/**.cpp" }

  end
