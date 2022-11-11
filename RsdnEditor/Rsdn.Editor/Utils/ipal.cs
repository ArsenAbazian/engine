using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Rsdn.Editor
{
	/// <summary>
	/// Platform Abstraction Layer Interface (интерфейс слой абстракции 
	/// от платформы). Служит для динамической смены класса классов абстрагирования
	/// от платформы.
	/// </summary>
	interface IPAL : IDisposable
	{
		void Initialization(Control control);

		void ShowCaret();
		void HideCaret();
		void DestroyCaret();
		void SetCaretPos(int x, int y);
		void SetCaretPos(Point location);
		void CreateCaret(int width, int height);

		void FreezeRepaint();
		void UnfreezeRepaint();
	}
}
