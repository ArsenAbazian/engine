using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Rsdn.Editor.Keyboard
{
	public partial class KeyboardEditorForm : Form
	{
		public KeyboardEditorForm(Rsdn.Editor.Edit edit)
		{
			if (edit == null)
				throw new ArgumentNullException("edit");

			InitializeComponent();

			_keyboardEditor.Init(edit);
		}
	}
}