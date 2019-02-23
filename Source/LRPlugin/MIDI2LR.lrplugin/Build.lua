--[[----------------------------------------------------------------------------

Build.lua

Takes Database.lua and produces text lists and other tools for documentation
and updating. Has to be run under Lightroom to be properly translated,
but is not used by users of the plugin.
 
This file is part of MIDI2LR. Copyright 2015 by Rory Jaffe.

MIDI2LR is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

MIDI2LR is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
MIDI2LR.  If not, see <http://www.gnu.org/licenses/>. 
------------------------------------------------------------------------------]]

local Database     = require 'Database'
local LrPathUtils  = import 'LrPathUtils'       

local menulocation = ""

local datafile = LrPathUtils.child(_PLUGIN.path, 'Commands.md')
local file = assert(io.open(datafile,'w'),'Error writing to ' .. datafile)
file:write([=[<!---
  This file automatically generated by Build.lua. To make persistent
  changes, edit Database.lua, not this file
-->
The tables below list all commands currently available in MIDI2LR for all submenus. The title row in each table corresponds with the name of the menu in the app. Controls marked *button* are intended to be used with a button or key, and unmarked controls are for faders or encoders.

*Note*: ※ symbol indicates that the command is undocumented and may not always behave as expected. Use cautiously.
]=])
for _,v in ipairs(Database.DataBase) do

  if v.Group ~= menulocation then
    menulocation = v.Group
    file:write("\n| "..menulocation.." |  |\n| ---- | ---- |\n")
  end
  local experimental = ""
  if v.Experimental  then 
    experimental = "\226\128\187"
  end
  file:write("| "..v.Translation..experimental.." | "..v.Explanation)
  if v.Type == 'button' then
    file:write(" *button*")
  end
  file:write(" Abbreviation: "..v.Command..". |\n" )

end
file:close()

datafile = LrPathUtils.child(_PLUGIN.path, 'GeneratedFromDatabase-ReadMe.txt')
file = assert(io.open(datafile,'w'),'Error writing to ' .. datafile)
file:write ("Running Build.lua generates files for the wiki: Commands.md. This file needs to replace the current file in the wiki.")
file:close()


