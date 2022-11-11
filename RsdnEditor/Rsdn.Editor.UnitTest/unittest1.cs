using System;

using Microsoft.VisualStudio.QualityTools.UnitTesting.Framework;
using System.Drawing;

using Rsdn.Editor;
using Rsdn.Editor.Formatter;

namespace Rsdn.Editor.UnitTest
{
	/// <summary>
	/// Summary description for UnitTest1
	/// </summary>
	[TestClass]
	public class UnitTest1
	{
		public UnitTest1()
		{
		}

		/// <summary>
		/// Initialize() is called once during test execution before
		/// test methods in this test class are executed.
		/// </summary>
		[TestInitialize()]
		public void Initialize()
		{
		}

		/// <summary>
		/// Cleanup() is called once during test execution after
		/// test methods in this class have executed unless the
		/// corresponding Initialize() call threw an exception.
		/// </summary>
		[TestCleanup()]
		public void Cleanup()
		{
		}

		[TestMethod]
		public void CalcRowHeightTest()
		{
			Font font = new Font("Arial", 10);
			FontHelper fh = FontHelper.GetFontHelper(font);

			Row row = new DocumentRow("");

			int res = row.CalcRowHeight(fh);

			Assert.AreNotEqual(res, 0, "CalcRowHeight was not set correctly.");
		}
	}
}
