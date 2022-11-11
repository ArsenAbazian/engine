using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Rsdn.Editor.Test
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			Application.EnableVisualStyles();

			if (args.Length > 1)
			{
				MessageBox.Show("�������� ���������� ����������.\r\n"
					+ "�������������: Rsdn.Editor.exe <���� � �����>", "Rsdn.Editor",
					MessageBoxButtons.OK, MessageBoxIcon.Stop);

				return;
			}

			Application.Run(args.Length == 1 ? new MainForm(args[0]) : new MainForm());
		}
	}
}