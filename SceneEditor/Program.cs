using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Globalization;
using System.Reflection;

namespace sceneEditor {
	static class Program {
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			UpdateCulture();
            
            Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
		}

		static void UpdateCulture() {
			
			CultureInfo currCulture = CultureInfo.CurrentCulture.Clone() as CultureInfo;
			currCulture.NumberFormat.CurrencyDecimalSeparator = ".";
			currCulture.NumberFormat.NumberDecimalSeparator = ".";
			currCulture.NumberFormat.PercentDecimalSeparator = ".";

			Application.CurrentCulture = currCulture;
		}
	}
}