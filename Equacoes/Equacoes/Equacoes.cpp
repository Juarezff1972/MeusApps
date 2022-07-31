#include "pch.h"
#include "resource.h"
#include "MyForm.h"

#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Equacoes::MyForm form;
	Application::Run(% form);
	return 0;
}
