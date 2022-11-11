using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Reflection;
using Pair = System.Collections.Generic.KeyValuePair<string, string>;

namespace Rsdn.Editor
{
	/// <summary>
	/// ������-����� ����������� ���������� � ������������ ����������� �� 
	/// ����� �������� ��� �������� � ������������� ��� �� ���������� �������.
	/// ����� ������������� ����� ������������ ���������� ������� �����
	/// �������� ��������-����������� ������������ �������.
	/// ���� ������ ���������� KeyboardShortcutsMap.xml.
	/// ���� � ��������� ������������ ���������� ����� ����������
	/// � �������� %APPDATA%\Rsdn.Editor ��� �������� ��� ������������� ������
	/// � �������� ��������� ������� ����������.
	/// ���� ���� �� ������ � ����� �� ���� ���������, �� �������� 
	/// ������������ ���������� ������� �� ��������.
	/// </summary>
	public class KeyboardShortcutsMap
	{
		/// <summary>
		/// ��� ������ ����������� ��� ������������ ����������.
		/// </summary>
		public delegate void KeyHandler();

		public KeyboardShortcutsMap(object controller, string mapFileName)
		{
			StringBuilder errors = new StringBuilder(); // ��������� �� �������

			foreach (Pair pair in GetShortcuts(
				controller.GetType(), mapFileName, errors))
			{
				try
				{
					// ������� ������������ ���������� ���� ������� � ��������������
					// ����� ���������� ���������� �������.
					SetKeyboardHandler(
						ParseKeys(pair.Key), // ������������ ����������
						(KeyHandler)Delegate.CreateDelegate(
							typeof(KeyHandler), controller, 
							pair.Value, // Action (�������� ������)
							false, true));
				}
				catch (Exception ex)
				{
					errors.AppendLine(
						"���������� �������� ������������ ���������� '"
						+ pair.Key + "' � ������������ '"
						+ pair.Value + "'. " + ex.Message);
				}
			}
		}

		public static IEnumerable<Pair> GetShortcuts(
			Type controllerType, 
			string mapFileName,
			StringBuilder errors)
		{
			if (errors == null)
				throw new ArgumentNullException("errors");

			//DataSet shortcutsDataSet = new DataSet();
			//shortcutsDataSet.ReadXml(OpenReader(controller.GetType(), mapFileName));
			//DataTable shortcutsTable = shortcutsDataSet.Tables["Shortcuts"];

			//foreach (DataRow row in shortcutsTable.Rows)
			//{
			//  string key = (string)row["Key"];
			//  string action = (string)row["Action"];
			//  ...
			//}

			using (XmlReader reader = OpenReader(controllerType, mapFileName))
			{
				// ��������� ������ ��� �<?xml version="1.0" encoding="utf-8" ?>�
				if (!reader.Read() || reader.Name != "xml")
					throw new ApplicationException(
						"�������� ������ ����� KeyboardShortcutsMap.xml");

				// ���������� �������� ��������� ������ ������.
				while (reader.Read() && string.IsNullOrEmpty(reader.Name))
					;

				// ������� ��� ������ ���� "Shortcuts".
				if (reader.Name != "Shortcuts")
					throw new ApplicationException(
						"�������� ������ ����� KeyboardShortcutsMap.xml");

				// ������ ������ ����� ��������� � "Shortcuts".
				while (reader.Read())
				{
					// ������������ ��� ���� "Shortcut" ��������� ����� ������.
					if (reader.Name == "Shortcut")
					{
						string key = null;
						string action = null;

						try
						{
							key = reader.GetAttribute("Key"); // ������ �����
							action = reader.GetAttribute("Action"); // ������ ��� ��������
						}
						catch (Exception ex)
						{
							errors.AppendLine(
								"���������� ������� " 
								+ (key == null ? "������� 'Key' " : "������� 'Action'. ")
								+ ex.Message);
						}

						yield return new Pair(key, action);
					}
				}

				if (errors.Length > 0)
					MessageBox.Show(Form.ActiveForm,
						"��� ���������� ����� � ������������� ������������ ���� "
						+ "���������� ��������� ������: "
						+ Environment.NewLine + errors, Utils.AppName,
						MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}
		}

		/// <summary>
		/// ��������� XML-���� ���������� �������� ������������ ����������
		/// �� ������.
		/// </summary>
		private static XmlReader OpenReader(
			Type controllerType, 
			string mapFileName)
		{
			XmlReader reader;

			string path = Environment.GetFolderPath(
				Environment.SpecialFolder.ApplicationData);

			path = Path.Combine(Path.Combine(path, Utils.AppName), mapFileName);

			// ���� ���� � ������������� ����������� ���������� � ����� 
			// "Application Data" (%APPDATA%), ��� � ����� ��� ����������� DLL-�, ��
			// ������ ��������� �� ����. ����� ������� ��������� �� ����� 
			// ������������ ��� ���������� � ��������.
			if (File.Exists(path))
				reader = XmlReader.Create(path);
			else if (File.Exists(path = Path.Combine(
				GetModulePath(controllerType), mapFileName)))
				reader = XmlReader.Create(path);
			else
				reader = XmlReader.Create(new StringReader(
					Properties.Resources.KeyboardShortcutsMap));
			return reader;
		}

		/// <summary>
		/// ���������� ���� � ������������ ������ � ������� ��������� ���
		/// ������ <paramref name="type"/>.
		/// </summary>
		/// <param name="type">
		/// ��� ��� �������� ����� ���������� � ����� ������ �� ���������.
		/// </param>
		public static string GetModulePath(Type type)
		{
			return Path.GetDirectoryName(new Uri(type.Assembly.CodeBase).LocalPath);
		}

		/// <summary>
		/// ����������� ������ ���������� ����� ������� � Keys.
		/// ����� ������� ����� ������������ �� ��� (������ "|").
		/// </summary>
		/// <example>"Shift | Control | Right"</example>
		/// <param name="key">������������ ���������� � ���� ������.</param>
		private static Keys ParseKeys(string key)
		{
			Keys keys = 0;
			// ��������� ����� �� ��������� ��������
			string[] keyStrs = key.Split('|');

			foreach (string value in keyStrs)
				keys |= (Keys)Enum.Parse(typeof(Keys), value);
			return keys;
		}

		/// <summary>
		/// ���������� �������-���������� ������� �������������� �������������
		/// ���������� ��� null.
		/// </summary>
		/// <param name="shortcut">������������ ����������.</param>
		/// <returns>������� ����������.</returns>
		public KeyHandler this[Keys shortcut]
		{
			get
			{
				KeyHandler keyHandler;

				if (_kbdMap.TryGetValue(shortcut, out keyHandler))
					return keyHandler;

				return null;
			}
		}

		/// <summary>
		/// ��������������� ����� ����������� ��������� ������������� _kbdMap.
		/// ����������� ������������ ���������� � ��� ����������.
		/// </summary>
		/// <param name="keyData">������������ ����������.</param>
		/// <param name="keyHandler">����������.</param>
		private void SetKeyboardHandler(Keys keyData, KeyHandler keyHandler)
		{
			if (_kbdMap.ContainsKey(keyData))
				Trace.WriteLine("The keyboard shortcut '" + keyData
					+ "' be found twice.");

			_kbdMap[keyData] = keyHandler;
		}

		/// <summary>
		/// ������������� ������ ������ ������� �������� ������������ ����������,
		/// � ��������� ���������� ����������� ��� ������� ����� ������������� 
		/// ����������.
		/// </summary>
		private Dictionary<Keys, KeyHandler> _kbdMap =
			new Dictionary<Keys, KeyHandler>();
	}
}
