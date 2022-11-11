using System;
using Rsdn.Editor.Formatter;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel;
using System.Drawing;

namespace Rsdn.Editor
{
	/// <summary>
	/// Строка текста. Она может отображаться в редакторе напрямую
	/// или быть преобразована (на время отображения) в FoldedRow
	/// или HyphenationRow.
	/// </summary>
	public abstract class Row
	{
		protected internal static AppliedStyle[] _defaultPosStyleArray = 
			new AppliedStyle[0];

		/// <summary>Текст строки.</summary>
		public abstract string Text { get; set; }
		/// <summary>Длинна текста строки.</summary>
		public abstract int TextLength { get; }
		/// <summary>
		/// Сдвиг строки относительно строки документа. 
		/// Всегда 0 для строк документа.
		/// </summary>
		public abstract int OffsetInDocumentRow { get; }
		/// <summary>
		/// Список стилей разбитый на диапазоны. Если несколько стилей 
		/// пересекаются, создается диапазаон в котором используется сурогатный
		/// стиль.
		/// </summary>
		public abstract AppliedStyle[] AppliedStyles { get; set; }
		/// <summary>
		/// Для строк документа это ссылка на саму себя. Для остальных строк
		/// это ссылка на строку документа которая породила данную виртуальную
		/// строку.
		/// </summary>
		public abstract DocumentRow RealDocumentRow { get; }

		/// <summary>
		/// Вычисляет ширину строки.
		/// </summary>
		/// <param name="defaultFontHelper">
		/// Ссылка на описание шрифта используемого для расчета ширины текста
		/// для которого шрифт не задается стилем.
		/// </param>
		/// <returns></returns>
		internal int CalcRowHeight(View vieu)
		{
			AppliedStyle[] appliedStyles = AppliedStyles;

			float zoom = vieu.Zoom;
			CompleteFont font = vieu.DefaultStyle.Font;
			int h = ZoomedFont.Create(font, zoom).FontHelper.Height;

			foreach (AppliedStyle appliedStyle in appliedStyles)
			{
				Style style = appliedStyle.Style;
				DisplayImage displayImage = style.DisplayImageHandler;

				if (displayImage != null)
					h = Math.Max(h, displayImage(Text.Substring(appliedStyle.Pos,
						appliedStyle.Length)).Height);
				else if (style.Font != null)
				{
					font = PartialFont.Combine(style.Font, font);

					FontHelper fontHelper = ZoomedFont.Create(font, zoom).FontHelper;

					if (fontHelper != null)
						h = Math.Max(h, fontHelper.Height);
				}
			}

			return h;
		}
		
		/// <summary>
		/// Позиция начала строки которая бы была в случае раскрытия табуляций
		/// и других невидимых символов.
		/// У обычных строк эта пазиция равна нулю.
		/// </summary>
		[DebuggerHidden]
		public abstract int ViewPosition { get; }

		/// <summary>
		/// Возвращает текст строки с подставленными невидимыми символами.
		/// </summary>
		/// <param name="view">
		/// Представление у которого нужно брать информацию о невидимых символах
		/// и необходимости их отображения.
		/// </param>
		/// <returns>
		/// Строка с подставленными невидимыми символами.
		/// </returns>
		[DebuggerHidden]
		public virtual string GetExpandedText(IView view)
		{
			//TODO: Нужно изменить рассчет и отображение табуляции.
			return Utils.ExpandTabs(Text, ViewPosition, view.TabSize,
				view.ShowInvisibles, IsEndLine);
		}

		/// <summary>
		/// Говорит о том содержит ли данная строка окончание строки
		/// документа.
		/// Это свойство всегда true у строк документа.
		/// </summary>
		[DebuggerHidden]
		internal virtual bool IsEndLine { get { return true; } }

		/// <summary>
		/// Реализация паттерна Посититель.
		/// </summary>
		/// <param name="visitor">Посетитель.</param>
		public abstract void AcceptVisitor(IRowVisitor visitor);

		/// <summary>
		/// Метод ToString в основном нужен для отладки. VS 2005 по умолчанию
		/// использует его для отображения содержимого объекта.
		/// </summary>
		/// <returns></returns>
		public override string ToString()
		{
			// Закоментировано в связи с ошибкой в отладчике VS 2005.
			//return Utils.ExpandTabs(Text, Position<IView>, 2, true, true);
			return Text;
		}
	}
}
