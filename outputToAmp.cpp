/*
Copyright (C) 2011 Georgia Institute of Technology

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A mARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/*
* Generates square pulse current commands.
*/

#include <outputToAmp.h>

extern "C" Plugin::Object *createRTXIPlugin(void) {
	return new OtoA();
}

static DefaultGUIModel::variable_t vars[] =
{
	{ "IAInput", "", DefaultGUIModel::INPUT, },
	{ "ProtocolInput", "", DefaultGUIModel::INPUT, },
	{ "Output_To_Amp", "", DefaultGUIModel::OUTPUT, },
	{ "IA_Scale", "Scalar for IA Input", DefaultGUIModel::PARAMETER, },
	{ "Toggle_Channel","", DefaultGUIModel::PARAMETER },
	{ "Total_Output_Scale", "Scalar for total output", DefaultGUIModel::PARAMETER, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

OtoA::OtoA(void) : DefaultGUIModel("Output to Amp", ::vars, ::num_vars),IA_Scale(1),Total_Scale(.5e-3) {
	setWhatsThis("<p><b>I-Step:</b><br>This module sums input from IA_Calc and ia-activate and sends one output to the amplifier.</p>");
	createGUI(vars, num_vars);
	update(INIT);
	refresh();
	resizeMe();
}

OtoA::~OtoA(void) {}

void OtoA::execute(void) {
	IA_Input = input(0); 
	ProtocolInput = input(1); //protocol input is in mV

	if (Toggle_Channel==1) // For M_Neuron_Dave 
	{  
		ProtocolInput = ProtocolInput * 2*1e3; //scale protocol input to V 
	}

	output(0) = (Total_Scale*ProtocolInput)+(IA_Input*IA_Scale);
		

}

void OtoA::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			setParameter("IA_Scale", IA_Scale);
			setParameter("Total_Output_Scale", Total_Scale);
			setParameter("Toggle_Channel", 0);

			break;

		case MODIFY:
			IA_Scale = getParameter("IA_Scale").toDouble();
			Total_Scale = getParameter("Total_Output_Scale").toDouble();
			Toggle_Channel = getParameter("Toggle_Channel").toInt();
			break;

		case PAUSE:
			output(0) = 0;
	
		case PERIOD:
	
		default:
			break;
	}
		
}
