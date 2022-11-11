// Работа с клавиаторой.

using System.Collections.Generic;
using System.Windows.Forms;
using System.Diagnostics;

namespace Rsdn.Editor
{
	partial class View
	{
		/// <summary>
		/// Ассоциативный массив ключем котрого является клавиатурное сокращения,
		/// а значением обработчик выполняемый при нажатии этого клавиатурного 
		/// сокращения.
		/// </summary>
		private KeyboardShortcutsMap _keyboardMap;
		public const string KeyboardShortcutsFileName = "KeyboardShortcutsMap.xml";

		void InitKeyboard()
		{
			_keyboardMap = new KeyboardShortcutsMap(this, KeyboardShortcutsFileName);
		}

		protected override void OnKeyDown(KeyEventArgs e)
		{
			if (e.KeyValue >= 16 && e.KeyValue <= 18) // Alt, Control, Shift
				return;

			// Получаем обработчик ассоциирванный с текущим клавиатурным сокращением.
			KeyboardShortcutsMap.KeyHandler keyDownHandler = _keyboardMap[e.KeyData];

			if (keyDownHandler != null)
			{
				// Вызваем рельный обработчкик клавиатурного сокращения.
				keyDownHandler();
				// Подавляем дальнейшую обработку клавиатурного сокращения.
				e.SuppressKeyPress = true;
			}
			else
				base.OnKeyDown(e);
		}

		protected override void OnKeyPress(KeyPressEventArgs e)
		{
            _viewOwner.ProcessViewKeyPress(e);
            if (e.Handled) return;

            Debug.WriteLineIf(TraceKeyboard.TraceVerbose,
				string.Format("OnKeyPress: '{0}', category {1}",
					e.KeyChar, char.GetUnicodeCategory(e.KeyChar)));

			// Фильтруем всякие Esc, Ctrl+Z и тому подобное
			if (Utils.IsPrintable(e.KeyChar))
			{
				Document.Replace(e.KeyChar.ToString(),
					_selectionStartDocument, _selectionEndDocument);
			}
		}

		protected override bool IsInputKey(Keys keyData)
		{
			return true; // Иначе контрол не отдаст стрелки и т.п.
		}
	}
}
