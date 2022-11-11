// Отрисовка

//#define DBG // вкючает режим отладки казкраски концов строк.
//#define DBG_PAINT_BACKGROUND // Включает режим предварительного закрашивания фона.
//#define DBG_PAINT_LINE

using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

using Rsdn.Editor.Formatter;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		#region Переменные, константы и инициализация.

		const KnownColor KnownColorMinValue = KnownColor.Black; //KnownColor.ActiveBorder;
		const KnownColor KnownColorMaxValue = KnownColor.MenuHighlight;

		ImageAnimatorHelper _imageAnimatorHelper;

		private void InitPaint()
		{
			_imageAnimatorHelper = new ImageAnimatorHelper(PaintAnimatedImages);
		}
		
		#endregion

		protected override void OnPaintBackground(PaintEventArgs pevent) { }

		#region OnPaint

		protected override void OnPaint(PaintEventArgs e)
		{
			//Debug.WriteLine("View.OnPaint()");
			//CheckForNecessaryRepaint();
			if (!IsNecessaryRepaint())
				return;

			_imageAnimatorHelper.Redraw = false;

			AllTimeStart();
#if PERF_TEST
			_paintRowTime = 0;
			_makeStyledRangesTime = 0;
			_expandTabsTime = 0;
			_paintStyledRangeTime = 0;
			_measureWidthTime = 0;
			_drawTextTime = 0;
#endif // PERF_TEST

			_imageAnimatorHelper.Clear();

			PaintInfo pi = new PaintInfo();
			pi.DefaultStyle = DefaultStyle;
#if DBG_PAINT_LINE
			pi.CurrentLineColor = KnownColorMinValue;
#endif

			Graphics graphics = e.Graphics;
			Rectangle clipRect = Rectangle.Truncate(graphics.ClipBounds);
			pi.SelectionBackColor = Focused
				? Color.FromKnownColor(KnownColor.Highlight)
				: Color.FromKnownColor(KnownColor.InactiveCaption);

			using (pi.RsdnGraphics = new RsdnGraphics(graphics))
			using (pi.SelectionBrush = new SolidBrush(pi.SelectionBackColor))
			using (pi.Brush = new SolidBrush(BackColor))
			{
				#region Инициализация

				pi.Graphics = graphics;

				Rectangle textAreaRect = TextAreaRectangle;
				pi.RsdnGraphics.SetClip(Rectangle.Intersect(clipRect, textAreaRect));

				RowCollection rows = Rows;
				int count = rows == null ? 0 : rows.Count;

				#endregion

#if DBG_PAINT_BACKGROUND
				using (Brush brush1 = new SolidBrush(Color.Aquamarine))
					pi.Graphics.FillRectangle(brush1, ClientRectangle);
#endif

				#region Отрисовка текста строк

				int i = FirstVisibleRow;

				for (; i < count; i++)
				{
					if (pi.TopOffset > textAreaRect.Bottom)
						break;

					pi.Row = rows[i];
					pi.RowViewIndex = i;

					PaintRow(pi, textAreaRect);
				}

				#endregion

				#region Отрисовка полосы состояния

				textAreaRect = this.ClientRectangle;
				textAreaRect.X = 0;
				textAreaRect.Width = StateBarWidth;
				pi.RsdnGraphics.SetClip(Rectangle.Intersect(clipRect, textAreaRect));

				textAreaRect.Width = StateBarWidth - 1;
				pi.RsdnGraphics.FillSolidRect(textAreaRect, 
					Color.FromKnownColor(KnownColor.ButtonFace));

				textAreaRect.X = StateBarWidth - 1;
				textAreaRect.Width = 1;
				pi.RsdnGraphics.FillSolidRect(textAreaRect, 
					Color.FromKnownColor(KnownColor.ButtonHighlight));

				#endregion

				#region Закрашивание нижней области окна незаполненной текстом

				if (pi.TopOffset < textAreaRect.Height)
				{
					pi.RsdnGraphics.SetClip(clipRect);
					textAreaRect = ClientRectangle;
					textAreaRect.X += StateBarWidth;
					textAreaRect.Y = pi.TopOffset;
					textAreaRect.Height -= pi.TopOffset;

					pi.RsdnGraphics.FillSolidRect(textAreaRect, pi.DefaultStyle.BackColor);
				}

				#endregion

				DisplayPerfInfo(i - FirstVisibleRow);
			}

			foreach (ImageInfo imageInfo in pi.ImageInfos)
				pi.Graphics.DrawImage(imageInfo.Image, imageInfo.Bounds.Location);

			_imageAnimatorHelper.Redraw = AnimateImages;
		}
		
		#endregion

		#region PaintRow (Отрисовка вируальной строки).

		private void PaintRow(PaintInfo pi, Rectangle clientRect)
		{
			DrawRowTimeStart();

			pi.ViewPosition = pi.Row.ViewPosition;

			pi.RowPos = new Point(-HorizontalScrollOffset + clientRect.X,
				pi.TopOffset + clientRect.Y);

			PaintStyledRanges(pi);

			// Если нужно отисовывать символ конца строки...
			if (pi.Row.IsEndLine)
				PintEolVisualizer(pi);

			int right = clientRect.Right;
			clientRect.Y = pi.TopOffset;
			clientRect.Height = pi.RowMaxHeight;
			clientRect.X = pi.RowPos.X;
			clientRect.Width = right - clientRect.X - 0;

#if DBG
			pi.Graphics.FillRectangle(new SolidBrush(Color.FromKnownColor(c)), clientRect);
#else
			pi.RsdnGraphics.FillSolidRect(clientRect, pi.DefaultStyle.BackColor);
#endif
#if DBG
			c = (KnownColor)(((int)c) + 1);
			if (c > KnownColor.MenuHighlight)
				c = KnownColor.ActiveBorder;
#endif

			PaintDebugLine(pi, false);
			pi.TopOffset += pi.RowMaxHeight;
			DrawRowTimeFinish();
		}
		
		#endregion

		#region PaintAnimatedImages (Отрисовка анимации картинок).

		private void PaintAnimatedImages(List<ImageInfo> animatedImagesInfo)
		{
			ImageAnimator.UpdateFrames();

			Color highlight = Color.FromKnownColor(KnownColor.Highlight);
			Color selectionBackColor = Focused
				? Color.FromKnownColor(KnownColor.Highlight)
				: Color.FromKnownColor(KnownColor.InactiveCaption);

			using (Brush selectionBrush = new SolidBrush(selectionBackColor))
			using (Brush normalBrush = new SolidBrush(DefaultStyle.BackColor))
			using (Graphics g = CreateGraphics())
			{
				RectangleF clipRect = g.ClipBounds;
				Rectangle textAreaRect = TextAreaRectangle;
				g.SetClip(RectangleF.Intersect(clipRect, textAreaRect));

				foreach (ImageInfo info in animatedImagesInfo)
				{
					Point pt = info.Bounds.Location;
					Brush brush = info.BackColor == highlight
						? selectionBrush : normalBrush;

					pt.Y += info.Bounds.Height - info.Image.Height;
					g.FillRectangle(brush, info.Bounds);
					g.DrawImage(info.Image, pt);
				}
			}
		}
		
		#endregion

		#region PaintStyledRanges (Отрисовка стилевых диапазонов).

		private void PaintStyledRanges(PaintInfo pi)
		{
			MakeStyledRangesTimeStart();

			// Преобразуем стили из исходного формата к плоскому списку 
			// комбинированных стилей.
			StyledRange[] styledRanges = Utils.ToStyledRange(
				CalcAppliedStyles(pi), pi.DefaultStyle, pi.Row.Text, true);

			MakeStyledRangesTimeFinish();

			int endRangeIndex = styledRanges.Length - 1;

			// Рассчитываем максимальную высоту строки.
			pi.RowMaxHeight = pi.Row.CalcRowHeight(this);

			bool isFocused = Focused;

			for (int i = 0; i < styledRanges.Length; i++)
			{
				StyledRange styledRange = styledRanges[i];

				Style style = styledRange.Style;
				// Стиль предпологает отображение картинки!
				if (style.DisplayImageHandler != null)
				{
					Image img = style.DisplayImageHandler(styledRange.Text);
					Point pt = pi.RowPos;
					Rectangle imgRect = new Rectangle(pt, new Size(img.Width, pi.RowMaxHeight));
					// PB 2005-09-30 окончательно переехали на GDI
					//	pi.Graphics.FillRectangle(pi.GetBrush(style.BackColor), imgRect);
					Color backColor = isFocused ? style.BackColor : style.InactiveBackColor;
					pi.RsdnGraphics.FillSolidRect(imgRect, backColor);
					pt.Y += pi.RowMaxHeight - img.Height;

					if (ImageAnimator.CanAnimate(img))
						_imageAnimatorHelper.Add(img, imgRect, style.BackColor);

					pi.ImageInfos.Add(new ImageInfo(
						img, new Rectangle(pt, img.Size), style.BackColor));

					int w = img.Width;
					pi.Width += w;
					pi.RowPos.X += w;
				}
				else
				{
					ExpandTabsTimeStart(); //>>>>>>>

					int virtualPosition2 = pi.ViewPosition;

					pi.RangeText = Utils.ExpandTabs(styledRange.Text,
						ref virtualPosition2, TabSize, ShowInvisibles, false);

					pi.Style = style;

					ExpandTabsTimeFinish(); //<<<<<<<

					PaintStyledRangeTimeStart(); //>>>>>>>

					PaintStyledRange(pi);

					PaintStyledRangeTimeFinish(); //<<<<<<<
				}
			}

			pi.EndStyle = styledRanges.Length > 0
				? styledRanges[styledRanges.Length - 1].Style
				: pi.DefaultStyle;
		}

		private AppliedStyle[] CalcAppliedStyles(PaintInfo pi)
		{
			CheckSelectionIntegrity();

			AppliedStyle[] styles = pi.Row.AppliedStyles;
			Position<IView> firstPos = _selectionStartView.Min(_selectionEndView);
			Position<IView> lastPos  = _selectionStartView.Max(_selectionEndView);

			// Если индекс строки в представлении попадет в диапазон выделнных строк
			if (firstPos != lastPos 
				&& pi.RowViewIndex >= firstPos.Line
				&& pi.RowViewIndex <= lastPos.Line)
			{
				int start = pi.RowViewIndex == firstPos.Line
					? firstPos.Character : 0;
				int end = pi.RowViewIndex == lastPos.Line
					? lastPos.Character : pi.Row.TextLength;

				if (start > end)
					Utils.Swap(ref start, ref end);

				System.Array.Resize(ref styles, styles.Length + 1);
				styles[styles.Length - 1] = new AppliedStyle(
					_selectionStyle, start, end - start);
			}

			return styles;
		}

		[Conditional("DEBUG")]
		private void CheckSelectionIntegrity()
		{
			Position<IView> x = ToView(_selectionStartDocument);
			if (_selectionStartView != x)
			{
				Debugger.Break();
			}
			Position<IView> y = ToView(_selectionEndDocument);
			if (_selectionEndView != y)
			{
				Debugger.Break();
			}
		}
		
		#endregion

		#region PaintStyledRange (Отрисовка стиливого диапазона).

		private void PaintStyledRange(PaintInfo pi)
		{
			Style style = pi.Style;

			FontHelper fontHelper = 
				ZoomedFont.Create((CompleteFont)style.Font, Zoom).FontHelper;

			MeasureWidthTimeStart();

			int w = fontHelper.MeasureWidth(pi.RangeText, 0, pi.RangeText.Length,
				ref pi.ViewPosition, TabSize, ShowInvisibles);

			MeasureWidthTimeFinish();

			// Рассчитывам разницу между максимальной высотой строки и
			// выстой текущего шрифта.
			int fontHheightDelta = pi.RowMaxHeight - fontHelper.Height;
			Point point = pi.RowPos;
			bool isFocused = Focused;
			Color foreColor = isFocused ? style.ForeColor : style.InactiveForeColor;
			Color backColor = isFocused ? style.BackColor : style.InactiveBackColor;

			// Если текущий шрифт ниже максимальной высоты строки...
			if (fontHheightDelta > 0)
			{
				// Зарисовываем пространдство над текущим диапазаоном...
				Rectangle overRangeRect =
					new Rectangle(point, new Size(w, fontHheightDelta));
				pi.RsdnGraphics.FillSolidRect(overRangeRect, backColor);
				point.Y += fontHheightDelta; // и сдвигаем текст диапазона вниз.
			}


			DrawTextTimeStart();

			//TextRenderer.DrawText(pi.Graphics, pi.RangeText, style.Font, point,
			//	foreColor, backColor, Utils.FormatFlags);
			pi.RsdnGraphics.DrawText(pi.RangeText,
				ZoomedFont.Create((CompleteFont)style.Font, Zoom), point,
				foreColor, backColor);

			DrawTextTimeFinish();

			pi.Width += w;
			pi.RowPos.X += w;
		}
		
		#endregion

		#region PintEolVisualizer (Отрисовка конца строки).

		private void PintEolVisualizer(PaintInfo pi)
		{
			DocumentRow simpleRow = pi.Row.RealDocumentRow;
			Style endStyle = pi.EndStyle;

			pi.RangeText = ShowInvisibles ? Utils.EolVisualizer : " ";
			pi.Style = simpleRow.IsContinueStylingOnNextLine ? endStyle : pi.DefaultStyle;

			PaintStyledRange(pi);

		}
		
		#endregion

		#region IFreezeRepaint Members

		FreezeRepaintHelper FreezeRepaint
		{
			get { return new FreezeRepaintHelper(this); }
		}

		private int _freezeRepaintCounter;

		public bool IsFreezeRepaint
		{
			get { return _freezeRepaintCounter > 0; }
		}

		Position<IView> _oldCarretPosition;

		void IFreezeRepaint.FreezeRepaint()
		{
			if (_freezeRepaintCounter == 0)
				_oldCarretPosition = _selectionEndView;

			_freezeRepaintCounter++;
			Pal.FreezeRepaint();
		}

		void IFreezeRepaint.UnfreezeRepaint()
		{
			_freezeRepaintCounter--;
			Pal.UnfreezeRepaint();

			if (_freezeRepaintCounter == 0)
			{
				Invalidate();
				Update();

				if (_oldCarretPosition != _selectionEndView)
					SetCarretPosition(_selectionEndView);
			}
		}

		#endregion

		#region Отладочные функции.

		[Conditional("DBG_PAINT_LINE")]
		void PaintDebugMarker(ref int topOffset)
		{
			PaintInfo pi = new PaintInfo();
			pi.TopOffset = topOffset;

			using (pi.Graphics = Graphics.FromHwnd(Handle))
				PaintDebugLine(pi, true);

			topOffset++;
		}

		[Conditional("DBG_PAINT_LINE")]
		void PaintDebugLine(PaintInfo pi, bool isMarker)
		{
			int y = pi.TopOffset + pi.RowMaxHeight;

			const int MarkerSize = 20;

			Rectangle rect = TextAreaRectangle;
			Point start = new Point(rect.X, y);
			Point end = new Point(isMarker ? rect.X + MarkerSize : rect.Right, y);

#if DBG_PAINT_LINE
			using (Pen pen = new Pen(Color.FromKnownColor(
				/*isMarker ? KnownColor.Red :*/ pi.CurrentLineColor)))
				pi.Graphics.DrawLine(pen, start, end);

			Utils.IncremantKnownColor(pi.CurrentLineColor);
#endif

			pi.TopOffset++;
		}

#if DBG
		static KnownColor c = (KnownColor)0;
#endif

		#endregion
	}
}
