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
	/// Хэлпер-класс считывающий информацию о клавиатурных сокращениях из 
	/// файла настроек или ресурсов и преобразующий его во внутренний словарь.
	/// После инициализации можно пользоваться индексером объекта чтобы
	/// получать делегаты-обработчики клавиатурных событий.
	/// Файл должен называться KeyboardShortcutsMap.xml.
	/// Файл с описанием клавиатурных сокращений может находиться
	/// в каталоге %APPDATA%\Rsdn.Editor иди каталоге где располагаться сборка
	/// в котророй находится элемент управления.
	/// Если файл не найден в одном из этих каталогов, то описание 
	/// клавиатурных сокращений берется из ресурсов.
	/// </summary>
	public class KeyboardShortcutsMap
	{
		/// <summary>
		/// Тип метода вызываемого при клавиатурном сокращении.
		/// </summary>
		public delegate void KeyHandler();

		public KeyboardShortcutsMap(object controller, string mapFileName)
		{
			StringBuilder errors = new StringBuilder(); // сообщения об ошибках

			foreach (Pair pair in GetShortcuts(
				controller.GetType(), mapFileName, errors))
			{
				try
				{
					// Получам клавиатурное сокращение плюс делегат и инициализируем
					// этими значениями внутренний словарь.
					SetKeyboardHandler(
						ParseKeys(pair.Key), // Клавиатурное сокращение
						(KeyHandler)Delegate.CreateDelegate(
							typeof(KeyHandler), controller, 
							pair.Value, // Action (название метода)
							false, true));
				}
				catch (Exception ex)
				{
					errors.AppendLine(
						"Невозможно добавить клавиатурное сокращение '"
						+ pair.Key + "' с обработчиком '"
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
				// Считываем первый тег «<?xml version="1.0" encoding="utf-8" ?>»
				if (!reader.Read() || reader.Name != "xml")
					throw new ApplicationException(
						"Неверный формат файла KeyboardShortcutsMap.xml");

				// Пропускаем возможно имеющиеся пустые строки.
				while (reader.Read() && string.IsNullOrEmpty(reader.Name))
					;

				// Текущий тег должен быть "Shortcuts".
				if (reader.Name != "Shortcuts")
					throw new ApplicationException(
						"Неверный формат файла KeyboardShortcutsMap.xml");

				// Читаем список тегов вложенных в "Shortcuts".
				while (reader.Read())
				{
					// Обрабатываем все теги "Shortcut" пропуская любые другие.
					if (reader.Name == "Shortcut")
					{
						string key = null;
						string action = null;

						try
						{
							key = reader.GetAttribute("Key"); // читаем ключь
							action = reader.GetAttribute("Action"); // читаем имя делегата
						}
						catch (Exception ex)
						{
							errors.AppendLine(
								"Невозможно считать " 
								+ (key == null ? "атрибут 'Key' " : "атрибут 'Action'. ")
								+ ex.Message);
						}

						yield return new Pair(key, action);
					}
				}

				if (errors.Length > 0)
					MessageBox.Show(Form.ActiveForm,
						"При считывании файла с клавиатурными сокращениями были "
						+ "обнаружены следующие ошибки: "
						+ Environment.NewLine + errors, Utils.AppName,
						MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}
		}

		/// <summary>
		/// Открывает XML-файл содержащий описание клавиатурных сокращений
		/// на чтение.
		/// </summary>
		private static XmlReader OpenReader(
			Type controllerType, 
			string mapFileName)
		{
			XmlReader reader;

			string path = Environment.GetFolderPath(
				Environment.SpecialFolder.ApplicationData);

			path = Path.Combine(Path.Combine(path, Utils.AppName), mapFileName);

			// Если файл с клавиатурными настройками существует в папке 
			// "Application Data" (%APPDATA%), или в папке где расположена DLL-а, то
			// читаем настройки из него. Иначе чиатаем настройки из файла 
			// сохраненного при компиляции в ресурсах.
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
		/// Возвращает путь к исполняемому модулю в котором находится код
		/// класса <paramref name="type"/>.
		/// </summary>
		/// <param name="type">
		/// Тип для которого нужно определить в каком модуле он находится.
		/// </param>
		public static string GetModulePath(Type type)
		{
			return Path.GetDirectoryName(new Uri(type.Assembly.CodeBase).LocalPath);
		}

		/// <summary>
		/// Преобразует строку содержащую имена клавишь в Keys.
		/// Имена клавишь могут объеденяться по или (знаком "|").
		/// </summary>
		/// <example>"Shift | Control | Right"</example>
		/// <param name="key">Клавиатурное сокращение в виде строки.</param>
		private static Keys ParseKeys(string key)
		{
			Keys keys = 0;
			// Разбиваем ключь на отдельные значения
			string[] keyStrs = key.Split('|');

			foreach (string value in keyStrs)
				keys |= (Keys)Enum.Parse(typeof(Keys), value);
			return keys;
		}

		/// <summary>
		/// Возвращает делегат-обработчик события соотвествующий клавиатурному
		/// сокращению или null.
		/// </summary>
		/// <param name="shortcut">Клавиатурное сокращение.</param>
		/// <returns>Делегат обработчки.</returns>
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
		/// Вспомогательный метод позволяющий пуростить инициализацию _kbdMap.
		/// Ассоциирует клавиатурное сокращение и его обработчик.
		/// </summary>
		/// <param name="keyData">Клавиатурное сокращение.</param>
		/// <param name="keyHandler">Обработчик.</param>
		private void SetKeyboardHandler(Keys keyData, KeyHandler keyHandler)
		{
			if (_kbdMap.ContainsKey(keyData))
				Trace.WriteLine("The keyboard shortcut '" + keyData
					+ "' be found twice.");

			_kbdMap[keyData] = keyHandler;
		}

		/// <summary>
		/// Ассоциативный массив ключем котрого является клавиатурное сокращения,
		/// а значением обработчик выполняемый при нажатии этого клавиатурного 
		/// сокращения.
		/// </summary>
		private Dictionary<Keys, KeyHandler> _kbdMap =
			new Dictionary<Keys, KeyHandler>();
	}
}
