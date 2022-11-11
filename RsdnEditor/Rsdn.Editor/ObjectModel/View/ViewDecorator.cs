using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Rsdn.Editor.ObjectModel
{
	class ViewDecorator : UserControl, IView, IViewInternal
	{
		protected virtual IView Child
		{
			get { return null; }
			set { }
		}

		protected virtual IViewInternal ChildInternal
		{
			get { return (IViewInternal)Child; }
		}

		#region IView Members

		public new virtual Graphics CreateGraphics() { return Child.CreateGraphics(); }
		public virtual ISelection Selection { get { return Child.Selection; } }

		public virtual int TabSize
		{
			get { return Child.TabSize; }
			set { Child.TabSize = value; }
		}

		public virtual int FirstVisibleRow
		{
			get { return Child.FirstVisibleRow; }
			set { Child.FirstVisibleRow = value; }
		}

		public virtual int FirstVisibleCol
		{
			get { return Child.FirstVisibleCol; }
			set { Child.FirstVisibleCol = value; }
		}

		public virtual int HorizontalScrollStep
		{
			get { return Child.HorizontalScrollStep; }
			set { Child.HorizontalScrollStep = value; }
		}

		public virtual bool ShowInvisibles
		{
			get { return Child.ShowInvisibles; }
			set { Child.ShowInvisibles = value; }
		}

		public virtual CompleteStyle DefaultStyle
		{
			get { return Child.DefaultStyle; }
			set { Child.DefaultStyle = value; }
		}

		public virtual int TextAreaWidth
		{
			get { return Child.TextAreaWidth; }
		}

		public virtual Document Document
		{
			get { return Child.Document; }
			set { Child.Document = value; }
		}

		public virtual RowCollection Rows
		{
			get { return Child.Rows; }
		}

		public virtual bool WordWrap
		{
			get { return Child.WordWrap; }
			set { Child.WordWrap = value; }
		}

		public virtual float Zoom
		{
			get { return Child.Zoom; }
			set { Child.Zoom = value; }
		}
		public bool DoublleBuffering
		{
			get { return Child.DoublleBuffering; }
			set { Child.DoublleBuffering = value; }
		}

		public bool AnimateImages
		{
			get { return Child.AnimateImages; }
			set { Child.AnimateImages = value; }
		}


		public virtual HitTestInfo HitTest(System.Drawing.Point point, bool isUseBestFit)
		{
			return Child.HitTest(point, isUseBestFit);
		}

		public virtual Position<Document> ToDocument(Position<IView> viewPosition)
		{
			return Child.ToDocument(viewPosition);
		}

		public virtual Position<Document> ToDocument(System.Drawing.Point location)
		{
			return Child.ToDocument(location);
		}

		public virtual Position<IView> ToView(Position<Document> documentPosition)
		{
			return Child.ToView(documentPosition);
		}

		public virtual Range<IView> ToView(Range<Document> range)
		{
			return Child.ToView(range);
		}

		public virtual Rectangle? ToLocation(Position<IView> viewPosition)
		{
			return Child.ToLocation(viewPosition);
		}

		public virtual void TextUpdated() { Child.TextUpdated(); }

		public virtual void ResetViewInfo() { Child.ResetViewInfo(); }

		public virtual bool BeginTextUpdated(
			Position<Document> StartPosition, 
			Position<Document> endPosition)
		{
			return Child.BeginTextUpdated(StartPosition, endPosition);
		}

		public virtual void EndTextUpdated(
			Position<Document> StartPosition, 
			Position<Document> endPosition)
		{
			Child.EndTextUpdated(StartPosition, endPosition);
		}

		public virtual bool EnsureVisible(Position<Document> position)
		{
			return Child.EnsureVisible(position);
		}

		public virtual bool EnsureVisible(Position<IView> position)
		{
			return Child.EnsureVisible(position);
		}

		public virtual bool EnsureVisible(int viewLine)
		{
			return Child.EnsureVisible(viewLine);
		}

		public virtual void UpdatedView() { Child.UpdatedView(); }

		public new event MouseEventHandler MouseDown
		{
			add { Child.MouseDown += value; }
			remove { Child.MouseDown -= value; }
		}

		public new event EventHandler MouseEnter
		{
			add { Child.MouseEnter += value; }
			remove { Child.MouseEnter -= value; }
		}

		public new event EventHandler MouseHover
		{
			add { Child.MouseHover += value; }
			remove { Child.MouseHover -= value; }
		}

		public new event EventHandler MouseLeave
		{
			add { Child.MouseLeave += value; }
			remove { Child.MouseLeave -= value; }
		}

		public new event MouseEventHandler MouseMove
		{
			add { Child.MouseMove += value; }
			remove { Child.MouseMove -= value; }
		}

		public new event MouseEventHandler MouseUp
		{
			add { Child.MouseUp += value; }
			remove { Child.MouseUp -= value; }
		}

		public new event MouseEventHandler MouseWheel
		{
			add { Child.MouseWheel += value; }
			remove { Child.MouseWheel -= value; }
		}

		public event EventHandler<SelectionChangedEventArgs> SelectionChanged
		{
			add { Child.SelectionChanged += value; }
			remove { Child.SelectionChanged -= value; }
		}

		public event EventHandler<SelectionChangingEventArgs> SelectionChanging
		{
			add { Child.SelectionChanging += value; }
			remove { Child.SelectionChanging -= value; }
		}

		#endregion

		#region IViewInternal Members

		public IViewOwner ViewOwner
		{
			get { return ChildInternal.ViewOwner; }
			set { ChildInternal.ViewOwner = value; }
		}

		#endregion
	}
}
