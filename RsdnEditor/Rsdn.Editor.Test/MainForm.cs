using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using Rsdn.Editor.Keyboard;
using Rsdn.Editor.Test.Properties;
using System.Runtime.CompilerServices;

namespace Rsdn.Editor.Test
{
	public partial class MainForm : Form
	{
		public MainForm(string filePath) : this()
		{
			LoadFile(filePath);
		}

		public MainForm()
		{
			Stopwatch sw = new Stopwatch();
			sw.Start();

			InitializeComponent();

			Debug.WriteLine(string.Format("InitializeComponent() time {0}", sw.Elapsed));
		}

		private void rsdnEdit1_Load(object sender, EventArgs e)
		{

		}

		Dictionary<string, string> _autoCorrectMap;

		private void Form1_Load(object sender, EventArgs e)
		{
			InitZoomComboBox();

			InitAutoCorrect();

			_wordWrapButton.Checked = _rsdnEdit.WordWrap = Settings.Default.WordWrap;
			_toggleInvisibleButton.Checked = _rsdnEdit.ShowInvisibles 
				= Settings.Default.ShowInvisibles;

			Text = "IntPtr = "
				+ System.Runtime.InteropServices.Marshal.SizeOf(typeof(IntPtr))
				+ " byte.";

			string lastEditedFile = Settings.Default.LastEditedFile;

			if (lastEditedFile != null && lastEditedFile.Trim() != "" && File.Exists(lastEditedFile))
			{
				LoadFile(lastEditedFile);
			}
		}

		private void InitZoomComboBox()
		{
			const int ItemsCount = 40;
			object[] items = new object[ItemsCount];
			for (int i = 0; i < ItemsCount; i++)
				items[i] = ((double)(i + 1) / 10.0).ToString("0.0");
			_zoomComboBox.Items.AddRange(items);

			_zoomComboBox.Text = Settings.Default.Zoom.ToString("0.0");
		}

		private void LoadFile(string lastEditedFile)
		{
			string text = File.ReadAllText(lastEditedFile, Encoding.Default);
			PerfCounter timer = new PerfCounter();

			timer.Start();
			_rsdnEdit.Document.Text = text;
			_selectionToolLabel.Text = "File load time is " + timer.Finish();

			_path = lastEditedFile;
		}

		private void InitAutoCorrect()
		{
			string[] autoCorrect = File.ReadAllLines(
				Path.Combine(Rsdn.Editor.Utils.GetModulePath(this.GetType()),
				"AutoCorrect.txt"));

			_autoCorrectMap = new Dictionary<string, string>(autoCorrect.Length);

			foreach (string line in autoCorrect)
			{
				string[] pair = line.Split(';');
				_autoCorrectMap.Add(pair[0], pair[1]);
			}
		}

		private void Form1_MouseDown(object sender, MouseEventArgs e)
		{
			Capture = true;
		}

		private void Form1_MouseUp(object sender, MouseEventArgs e)
		{
			Capture = false;
		}

		private void Form1_MouseMove(object sender, MouseEventArgs e)
		{
			if (Capture)
			{
				_rsdnEdit.Height = e.Y - _rsdnEdit.Top;
				_rsdnEdit.Width = e.X - _rsdnEdit.Left;
				_rsdnEdit.Invalidate();
				_rsdnEdit.Update();
			}
		}

		private void Form1_DoubleClick(object sender, EventArgs e)
		{
			_rsdnEdit.ShowInvisibles = !_rsdnEdit.ShowInvisibles;
		}

		private void _showPropertiesMenuItem_Click(object sender, EventArgs e)
		{
			_showPropertiesMenuItem.Checked = !_showPropertiesMenuItem.Checked;
			
			if (_showPropertiesMenuItem.Checked)
				_propertyGrid.SelectedObject = _rsdnEdit;
			else
				_propertyGrid.SelectedObject = null;

			_propertyGrid.Visible = _showPropertiesMenuItem.Checked;
		}

		private string _path;

		private void OpenFile()
		{
			if (_openFileDialog.ShowDialog() == DialogResult.OK)
			{
				LoadFile(_openFileDialog.FileName);

				Properties.Settings.Default.LastEditedFile = _path;
			}
		}

		private void _openMenuItem_Click(object sender, EventArgs e)
		{
			OpenFile();
		}

		private void _exitMenuItem_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void _openButton_Click(object sender, EventArgs e)
		{
			OpenFile();
		}

		private void _wordWrapButton_Click(object sender, EventArgs e)
		{
			_wordWrapButton.Checked = !_wordWrapButton.Checked;
			_rsdnEdit.WordWrap = _wordWrapButton.Checked;
		}

		private void _toggleInvisibleButton_Click(object sender, EventArgs e)
		{
			_toggleInvisibleButton.Checked = !_toggleInvisibleButton.Checked;
			_rsdnEdit.ShowInvisibles = _toggleInvisibleButton.Checked;
		}

		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			switch (e.KeyData)
			{
				case Keys.Control | Keys.O:
					e.Handled = true;
					e.SuppressKeyPress = true;
					OpenFile();
					break;
				case Keys.Control | Keys.S:
					e.Handled = true;
					e.SuppressKeyPress = true;
					SaveFile(false);
					break;
			}
		}

		enum ThreeState
		{
			Yes = 1,
			No,
			Cancel,
		}

		private ThreeState SaveFile(bool promptSave)
		{
			DialogResult result;

			if (string.IsNullOrEmpty(_path))
			{
				if (promptSave)
				{
					result = MessageBox.Show(Form.ActiveForm,
						"Сохранить новый файл?", "Rsdn.Editor",
						MessageBoxButtons.YesNoCancel);

					if (result == DialogResult.Yes)
						return SaveAs();

					return result == DialogResult.Yes ? ThreeState.Yes
						: result == DialogResult.No ? ThreeState.No : ThreeState.Cancel;
				}
			}

			result = promptSave
					? MessageBox.Show(Form.ActiveForm,
						"Сохранить файл '" + _path + "'?", "Rsdn.Editor",
						MessageBoxButtons.YesNoCancel)
					: DialogResult.Yes;

			if (result == DialogResult.Cancel)
				return ThreeState.Cancel;

			if (result == DialogResult.Yes)
				return SaveFile();

			return ThreeState.No;
		}

		private ThreeState SaveAs()
		{
			_saveAsDialog.FileName = _path;

			if (_saveAsDialog.ShowDialog() == DialogResult.OK)
			{
				_path = _saveAsDialog.FileName;
				return SaveFile();
			}
			else
				return ThreeState.Cancel;
		}

		ThreeState SaveFile()
		{
			if (string.IsNullOrEmpty(_path))
				return SaveAs();

			try
			{
				File.WriteAllText(_path, _rsdnEdit.Document.Text, Encoding.Default);
				_rsdnEdit.Document.IsDirty = false;
				return ThreeState.Yes;
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, "Производша ошибка: " + ex.Message);
				return ThreeState.Cancel;
			}
		}

		private void _toggleAbimateButton_Click(object sender, EventArgs e)
		{
			_rsdnEdit.AnimateImages = _toggleAnimateButton.Checked;
		}

		private void _doublleBufferingButton_Click(object sender, EventArgs e)
		{
			_rsdnEdit.DoubleBuffering = _doublleBufferingButton.Checked;
		}

		private void _rsdnEdit_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			_selectionStatusLabel.Text = e.StartPosition + " - " + e.EndPosition;
		}

		private void Form1_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (e.KeyChar == 15)
				e.Handled = true;
		}

		private void _rsdnEdit_BeforeChangeText(object sender, BeforeChangeTexEventArgs e)
		{
			if (e.Text.Length == 1)
			{
				char ch = e.Text[0];

				if (!char.IsLetterOrDigit(ch) || ch == '\n')
				{
					string replaceWith;
					// Начало выделения.
					Position<Document> start = _rsdnEdit.ActiveView.Selection.Start;
					// Конец выделения.
					Position<Document> end = _rsdnEdit.ActiveView.Selection.End;
					// Минимальная позиция выделения (start может оказаться
					// больше end).
					Position<Document> min = start.Min(end);
					// Список строк документа.
					DocumentRowCollection rows = _rsdnEdit.Document.Rows;
					// Строка на которую указывает минимальная позиция выделения.
					string text = rows[min.Line].Text;

					// Если мы находимся в начале строки, то дополнять нечего.
					if (min.Character == 0)
						return;

					// Ищем крайний с лева от курсора непробельный символ или начало строки.
					int i = min.Character - 1;
					for (; i > 0; i--)
					{
						if (char.IsWhiteSpace(text[i]))
						{
							i++;
							break;
						}
					}

					// Выделяем слово подлежащее проверке на наличие в списке автозамен.
					string key = text.Substring(i, min.Character - i);
					// Ищем слово в списке автозамен.
					if (_autoCorrectMap.TryGetValue(key, out replaceWith)
						|| _autoCorrectMap.TryGetValue(key.ToLower(), out replaceWith))
					{
						// Если нашли, отменяем текущее редактирование...
						e.Cancel = true;
						// Расчитываем на сколько нужно сдвинуть курсор...
						int delta = -key.Length + replaceWith.Length;
						// Вставляем автозамену...
						_rsdnEdit.Document.ReplaceDirect(replaceWith,
							new Position<Document>(min.Line, i), min);
						// Сдвигаем курсор.
						start.Character += delta;
						end.Character += delta;
						// Вставляем текст вбитый пользователем.
						_rsdnEdit.Document.ReplaceDirect(e.Text, start, end);
						return;
					}
				}
			}
		}

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			float zoom;
			if (float.TryParse(_zoomComboBox.Text, out zoom))
				Settings.Default.Zoom = zoom;

			Settings.Default.WordWrap = _rsdnEdit.WordWrap;
			Settings.Default.ShowInvisibles = _rsdnEdit.ShowInvisibles;
			Settings.Default.Save();

			if (_rsdnEdit.Document.IsDirty)
				if (SaveFile(true) == ThreeState.Cancel)
					e.Cancel = true;
		}

		private void _keyboardSettingsButton_Click(object sender, EventArgs e)
		{
			KeyboardEditorForm keyboardEditorForm = new KeyboardEditorForm(_rsdnEdit);
			keyboardEditorForm.ShowDialog(this);
		}

		private void _newButton_Click(object sender, EventArgs e)
		{
			if (!_rsdnEdit.Document.IsDirty || SaveFile(true) != ThreeState.Cancel)
			{
				_path = "";
				_rsdnEdit.Document = new Document();
			}
		}

		private void _saveButton_Click(object sender, EventArgs e)
		{
			SaveFile(false);
		}

		private void _saveAsMenuItem_Click(object sender, EventArgs e)
		{
			SaveAs();
		}

		private void _rsdnEdit_DirtyChanged(object sender, EventArgs e)
		{
			_saveButton.Enabled = _rsdnEdit.Document.IsDirty;
			_saveMenuItem.Enabled = _rsdnEdit.Document.IsDirty;
		}

		private void _zoomComboBox_TextChanged(object sender, EventArgs e)
		{
			float res;

			if (float.TryParse(_zoomComboBox.Text, out res))
				_rsdnEdit.ActiveView.Zoom = res;
		}

		private void _testButton_Click(object sender, EventArgs e)
		{
			_repaintTimer.Enabled = !_repaintTimer.Enabled;
			_testButton.Checked = _repaintTimer.Enabled;
		}

		private void _repaintTimer_Tick(object sender, EventArgs e)
		{
			_rsdnEdit.ActiveView.ShowInvisibles =
				!_rsdnEdit.ActiveView.ShowInvisibles;
		}
    }
}