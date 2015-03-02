local BUILD = true

newaction {
	trigger = "clean",
	description = "Clean up temporary files",
	execute = function()
		os.rmdir("..\Build")
		os.rmdir("..\Bin")
	end
}

newaction {
	trigger = "clean-all",
	description = "Clean up temporary and binary files",
	execute = function()
		os.rmdir("../Build")
		os.rmdir("../Bin")
		os.rmdir("../Lib")
		--os.execute("cd ..\Bin&&del *.* /Q") --remove all files from /Bin/ but not folders
	end
}

newaction {
	trigger = "gen-docs",
	description = "Generate documentation",
	execute = function()
		os.execute("cd ../Docs&&doxygen Doxyfile.cfg")
	end
}

configurationS = "$(Configuration)"
targetPlatform = "$(PlatformTarget)"
toolset = "vc$(PlatformToolsetVersion)"

function suffix_macro( version, config )
	
	--TitaniumEagle_Player_v0.0.0_windows-x64-vc120_d
		--%{cfg.platform}
		--%{cfg.architecture}
		--$(PlatformToolset)
		--version
		--config suffix
	local suffix = ""
	if version then
		suffix = suffix .. "_v" .. version
	end
	
	--suffix = suffix .. string.format("_%s-%s-%s", "%{cfg.system}", targetPlatform, toolset )
	
	if config then
		local cfg = configuration() --clone configuration
		
		configuration{ cfg.terms, "Release" } --current config + debug
			targetsuffix ( suffix )
			
		configuration{ cfg.terms, "Debug", "not RelWithDebInfo" } --current config + debug
			suffix = suffix .. "_d"
			targetsuffix ( suffix )
			
		configuration{ cfg.terms, "RelWithDebInfo", "not Debug" } --current config + relwithdebinfo
			suffix = suffix .. "_rd"
			targetsuffix ( suffix )
	
		if cfg.terms then
			configuration{cfg.terms}
		else
			configuration{}
		end
	else
		targetsuffix ( suffix )
	end
	--print(suffix)
end

--[[
ENGINE DIR - SOURCE/BUILD(:D) FOLDERS
ex: Source/Engine/
]]
DIR_ROOT = "../../"
DIR_BIN 			= DIR_ROOT .. "Bin/"
DIR_BUILD 			= DIR_ROOT .. "Build/"
DIR_LIB 			= DIR_ROOT .. "Lib/"
DIR_SOURCE 			= DIR_ROOT .. "Source/"

if _ACTION == "clean" or
	_ACTION == "clean-all" or
	_ACTION == "version" or
	_ACTION == "gen-docs" then
	--do nothing
else

function configMacro()
	configuration "Release**"
		defines { "NDEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		optimize "On"
		
	configuration "Debug**"
		flags { "Symbols" }
		defines { "DEBUG=1", "_DEBUG=1", "_ITERATOR_DEBUG_LEVEL=2" }
		optimize "Off"
		
	configuration "RelWithDebInfo**"
		flags { "Symbols" }
		defines { "NDEBUG", "_ITERATOR_DEBUG_LEVEL=0" }
		optimize "On" -- or "Debug"?
		
	configuration { "gmake", "StaticLib" }
		buildoptions { "-fPIC" }
	
	configuration { "gmake" }
		buildoptions { "-std=c++11", "-Wno-unknown-pragmas", "-Wno-unused-parameter" }
		
	configuration {}
end

solution "ALL_BUILD"
	configurations {"Release", "Debug", "RelWithDebInfo" }
	platforms {"x32", "x64"}
	
	configMacro()
		
	configuration "vs*"
		--buildoptions { "/INCREMENTAL:NO" } --why not?
		defines { "_CRT_SECURE_NO_DEPRECATE" }
		defines { "_CRT_SECURE_NO_WARNINGS" }
		
	configuration "windows"
		defines { "WIN32", "_WIN32", "__OS=\"Windows\"" }

	configuration "macosx"
		defines { "MACOSX", "__OS=\"MacOSX\"" }

	configuration "linux"
		defines { "LINUX", "__OS=\"Linux\"" }
		
	configuration "x32"
		vectorextensions "SSE2" --Enable SSE2 cpu extensions
		--vectorextensions "AVX" --Enable AXV cpu extensions; 2011+ Bulldozer/Sandy Bridge
	
	configuration {}
		
	targetdir( "Lib" ) --put it in lib instead..
	location( "Build" )
	
project "KLib-Core"
	language "C++"
	files { "Source/Core/**.cpp", "Source/Core/**.hpp", "Include/KLib/**.*" }
	vpaths { ["Headers/*"] = "Include/KLib/**.*" } -- for including in IDEs

	includedirs { "Include" }
	
	kind ("StaticLib") --ideally this should be dynamic to reduce compile dependencies, but its no biggie
	
	configMacro()
	
	warnings "Extra" --Enable extra warnings. Warnings shouldn't exist
	
	suffix_macro ( nil, true )
	
end --if action == clean, clean-all, version; else.