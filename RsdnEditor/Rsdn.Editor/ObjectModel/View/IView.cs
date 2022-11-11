using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Rsdn.Editor.ObjectModel
{
	public interface IView
	{
		Graphics CreateGraphics();
		ISelection Selection { get; }
		int TabSize { get; set; }
		bool ShowInvisibles { get; set; }
		CompleteStyle DefaultStyle { get; set; }
		int TextAreaWidth { get; }
		Document Document { get; set; }
		RowCollection Rows { get; }
		bool WordWrap { get; set; }
		float Zoom { get; set; }
		int FirstVisibleRow { get; set; }
		int FirstVisibleCol { get; set; }
		int HorizontalScrollStep { get; set; }
		bool DoublleBuffering { get; set; }
		bool AnimateImages { get; set; }

		HitTestInfo HitTest(Point point, bool isUseBestFit);
		Position<Document> ToDocument(Position<IView> viewPosition);
		Position<Document> ToDocument(Point location);
		Position<IView> ToView(Position<Document> documentPosition);
		Range<IView> ToView(Range<Document> range);
		Rectangle? ToLocation(Position<IView> viewPosition);

		

		void TextUpdated();
		void ResetViewInfo();

		// Редактирование.
		bool BeginTextUpdated(Position<Document> StartPosition, Position<Document> endPosition);
		void EndTextUpdated(Position<Document> StartPosition, Position<Document> endPosition);

		#region EnsureVisible

		/// <summary>
		/// Проверят, видима ли позиция, переданная в качестве параметра. Если 
		/// позиция не видна во view, то она делается видимой. Если позиция 
		/// уже видна во view, то ничего не делается.
		/// </summary>
		/// <param name="position">Позиция, которая должна быть видимой.</param>
		/// <returns>True, если был произведен скролинг.</returns>
		bool EnsureVisible(Position<Document> position);
		/// <summary>
		/// Проверят, видима ли позиция, переданная в качестве параметра. Если 
		/// позиция не видна во view, то она делается видимой. Если позиция 
		/// уже видна во view, то ничего не делается.
		/// </summary>
		/// <param name="position">Позиция, которая должна быть видимой.</param>
		/// <returns>True, если был произведен скролинг.</returns>
		bool EnsureVisible(Position<IView> position);
		/// <summary>
		/// Проверят видима ли строка, переданная в качестве параметра. Если 
		/// строка не видна во view, то она делается видимой. Если строка
		/// уже видна во view, то ничего не делается.
		/// </summary>
		/// <param name="viewLine">Строка, которая должна быть видимой.</param>
		/// <returns>True, если был произведен скролинг.</returns>
		bool EnsureVisible(int viewLine);
		
		#endregion

		void UpdatedView();

		/// <summary>
		/// Occurs when the mouse pointer is over the control and a mouse button is
		/// pressed.
		/// </summary>
		event MouseEventHandler MouseDown;
		/// <summary>
		/// Occurs when the mouse pointer enters the control.
		/// </summary>
 		event EventHandler MouseEnter;
		/// <summary>
		/// Occurs when the mouse pointer hovers over the control.
		/// </summary>
 		event EventHandler MouseHover;
		/// <summary>
		/// Occurs when the mouse pointer leaves the control.
		/// </summary>
 		event EventHandler MouseLeave;
		/// <summary>
		/// Occurs when the mouse pointer is moved over the control.
		/// </summary>
 		event MouseEventHandler MouseMove;
		/// <summary>
		/// Occurs when the mouse pointer is over the control and a mouse button is
		/// released.
		/// </summary>
		event MouseEventHandler MouseUp;
		/// <summary>
		/// Occurs when the mouse wheel moves while the control has focus.
		/// </summary>
 		event MouseEventHandler MouseWheel;


		event EventHandler<SelectionChangedEventArgs> SelectionChanged;
		event EventHandler<SelectionChangingEventArgs> SelectionChanging;
	}
}
