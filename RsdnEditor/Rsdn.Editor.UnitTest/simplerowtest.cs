using Rsdn.Editor;
using Microsoft.VisualStudio.QualityTools.UnitTesting.Framework;
namespace Rsdn.Editor.UnitTest
{
	/// <summary>
	///This is a test class for Rsdn.Editor.SimpleRow and is intended
	///to contain all Rsdn.Editor.SimpleRow Unit Tests
	///</summary>
	[TestClass()]
	public class SimpleRowTest
	{


		private TestContext testContextInstance;

		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		public TestContext TestContext
		{
			get
			{
				return testContextInstance;
			}
			set
			{
				testContextInstance = value;
			}
		}

		/// <summary>
		///Initialize() is called once during test execution before
		///test methods in this test class are executed.
		///</summary>
		[TestInitialize()]
		public void Initialize()
		{
		}

		/// <summary>
		///Cleanup() is called once during test execution after
		///test methods in this class have executed unless
		///this test class' Initialize() method throws an exception.
		///</summary>
		[TestCleanup()]
		public void Cleanup()
		{
		}


		/// <summary>
		///A test case for Text
		///</summary>
		[TestMethod()]
		public void TextTest()
		{
			DocumentRow target = new DocumentRow();

			string val = null;

			target.Text = val;


			Assert.AreEqual(val, target.Text, "Rsdn.Editor.SimpleRow.Text was not set correctly.");
		}

	}


}
