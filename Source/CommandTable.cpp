/*
  ==============================================================================

  CommandTable.cpp

This file is part of MIDI2LR. Copyright 2015 by Rory Jaffe.

MIDI2LR is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

MIDI2LR is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
MIDI2LR.  If not, see <http://www.gnu.org/licenses/>.
  ==============================================================================
*/
#include "CommandTable.h"

#include <exception>
#include <memory>

#include <gsl/gsl_util>
#include "CommandTableModel.h"
#include "Misc.h"

CommandTable::CommandTable(const juce::String& component_name, CommandTableModel* model)
try : juce
   ::TableListBox{component_name, model}
   {
      auto head = std::make_unique<juce::TableHeaderComponent>();
      setHeader(head.release());
      getHeader().addColumn(juce::translate("MIDI Command"), 1, 150, 30, -1,
          juce::TableHeaderComponent::notResizable | juce::TableHeaderComponent::sortable);
      getHeader().addColumn(juce::translate("LR Command"), 2, 150, 30, -1,
          juce::TableHeaderComponent::notResizable | juce::TableHeaderComponent::sortable
              | juce::TableHeaderComponent::sortedForwards);
   }
catch (const std::exception& e) {
   rsj::ExceptionResponse(typeid(this).name(), __func__, e);
   throw;
}

bool CommandTable::keyPressed(const juce::KeyPress& k)
{
   try {
      const auto key_pressed = k.getKeyCode();
      if (key_pressed == juce::KeyPress::deleteKey) {
         if (getSelectedRow() != -1) {
            const auto last = getSelectedRow() == getNumRows() - 1;
            if (const auto ptr = dynamic_cast<CommandTableModel*>(getModel()))
               ptr->RemoveRow(gsl::narrow_cast<size_t>(getSelectedRow()));
            updateContent();
            if (last) // keep selection at the end
               selectRow(getNumRows() - 1);
            return true;
         }
         return false;
      }
      else if (key_pressed == juce::KeyPress::downKey) {
         if (getSelectedRow() != -1 && getSelectedRow() < getNumRows() - 1) {
            selectRow(getSelectedRow() + 1);
            return true;
         }
         return false;
      }
      else if (key_pressed == juce::KeyPress::upKey) {
         if (getSelectedRow() > 0 && getNumRows() > 1) {
            selectRow(getSelectedRow() - 1);
            return true;
         }
         return false;
      }
      else if (key_pressed == juce::KeyPress::pageUpKey) {
         if (getNumRows() > 0) {
            auto row = getSelectedRow() - 20;
            if (row < 0)
               row = 0;
            selectRow(row);
            return true;
         }
         return false;
      }
      else if (key_pressed == juce::KeyPress::pageDownKey) {
         if (getNumRows() > 0) {
            auto row = getSelectedRow() + 20;
            if (row >= getNumRows())
               row = getNumRows() - 1;
            selectRow(row);
            return true;
         }
         return false;
      }
      else if (key_pressed == juce::KeyPress::homeKey) {
         if (getNumRows() > 0) {
            selectRow(0);
            return true;
         }
         return false;
      }
      else if (key_pressed == juce::KeyPress::endKey) {
         if (getNumRows() > 0) {
            selectRow(getNumRows() - 1);
            return true;
         }
         return false;
      }
      return false;
   }
   catch (const std::exception& e) {
      rsj::ExceptionResponse(typeid(this).name(), __func__, e);
      throw;
   }
}