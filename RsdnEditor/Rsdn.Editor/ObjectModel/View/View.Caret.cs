// Работа с кареткой (текстовым курсором).

using System.Drawing;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		public void RecalcCarrenCaretPosition()
		{
			SetCarretPosition(_selectionEndView);
		}

		/// <summary>
		/// Позволяет установить позицию каретки.
		/// </summary>
		/// <param name="viewPosition">
		/// Виртуальная позиция в которую нужно установить каретку.
		/// </param>
		public void SetCarretPosition(Position<IView> virtualPosition)
		{
			//TODO: ToReal не дешевая операция, а она вызывается в нескольких местах.
			//надо бы подумать как это дело оптимизировать.
			//Position<Document> caretPosition = ToReal(viewPosition);
			//if (_caretPosition == caretPosition)
			//	return;

			//_caretPosition = caretPosition;

			Rectangle? rect = ToLocation(virtualPosition);

			if (rect != null)
			{
				Pal.DestroyCaret();
				Pal.CreateCaret(1, rect.Value.Height);
				Pal.SetCaretPos(rect.Value.Location);
				Pal.ShowCaret();
			}
		}

	}
}
