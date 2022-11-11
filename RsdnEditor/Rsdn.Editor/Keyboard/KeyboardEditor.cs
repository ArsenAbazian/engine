using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using Pair = System.Collections.Generic.KeyValuePair<string, string>;

namespace Rsdn.Editor.Keyboard
{
	public partial class KeyboardEditor : UserControl
	{
		public KeyboardEditor()
		{
			InitializeComponent();
		}

		public void Init(Edit edit)
		{
			if (edit == null)
				throw new ArgumentNullException("edit");

			_edit = edit;
			InitCommandsCombo();

			StringBuilder errors = new StringBuilder(); // сообщения об ошибках
			
			foreach (Pair pair in KeyboardShortcutsMap.GetShortcuts(
				typeof(View), View.KeyboardShortcutsFileName, errors))
			{
				//System.Diagnostics.Debug.WriteLine(pair.Key + " = " + pair.Value);
				//_dataGridView.DataSource
			}
		}

		private void InitCommandsCombo()
		{
			List<ViewCommandInfo> commands = new List<ViewCommandInfo>();

			foreach (MethodInfo method in typeof(View).GetMethods(
				BindingFlags.Public | BindingFlags.Instance))
			{
				if (method.GetParameters().Length > 0)
					continue;

				ViewCommandAttribute[] attrs = (ViewCommandAttribute[])
					method.GetCustomAttributes(typeof(ViewCommandAttribute), false);

				if (attrs.Length <= 0)
					continue;

				commands.Add(new ViewCommandInfo(method.Name, attrs[0].DisplayText));
			}

			//_comandName.DisplayMember = "DisplayText";
			//_comandName.ValueMember = "CommandName";
			//_comandName.ValueType = typeof(ViewCommandInfo);
			//_comandName.Items.AddRange(commands.ToArray());
		}

		private Edit _edit;

		public Edit Edit
		{
			get { return _edit; }
		}

		private void KeyboardEditor_Load(object sender, EventArgs e)
		{

		}

		private void _dataGridView_DataError(object sender, DataGridViewDataErrorEventArgs e)
		{

		}
	}
}
