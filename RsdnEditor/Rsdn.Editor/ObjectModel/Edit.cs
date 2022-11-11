using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Rsdn.Editor.Formatter;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	public partial class Edit : UserControl, IViewOwner
	{
		public Edit()
		{
			SetStyle(ControlStyles.AllPaintingInWmPaint
							| ControlStyles.UserPaint
							| ControlStyles.ContainerControl, true);
			InitializeComponent();
			_views = new IView[] { _activeView = CreateView() };
			Control view = (Control)_views[0];
			view.Parent = this;
			Document = CreateDocument();
		}

        public virtual Document CreateDocument() { 
            return new Document();
        }

		protected virtual IView CreateView()
		{
			ViewScrollDecorator view = new ViewScrollDecorator();
			view.ViewOwner = this;
			view.Dock = DockStyle.Fill;
			view.Enter += delegate(object sender, EventArgs e)
			{ SetActiveView((IView)sender); };
			view.SplitterGripClicked += SplitterGripClicked;
			
			if (Document != null)
				view.Document = Document;

			return view;
		}

		void SplitterGripClicked(object sender, EventArgs e)
		{
			Split = true;
		}


		private Document _document;

		public Document Document
		{
			[DebuggerHidden]
			get { return _document; }
			set
			{
				if (_document != null)
					_document.DirtyChanged -= OnDirtyChanged;

				if (value == null)
					throw new ArgumentNullException("value");

				value.DirtyChanged += OnDirtyChanged;

				foreach (IView view in Views)
					view.Document = value;

				_document = value;

				foreach (IView view in Views)
					view.ResetViewInfo();

				OnDirtyChanged(_document, EventArgs.Empty);
			}
		}

		protected virtual void OnDirtyChanged(object sender, EventArgs e)
		{
			if (DirtyChanged != null)
				DirtyChanged(this, EventArgs.Empty);
		}

		public event EventHandler DirtyChanged;

		private bool ShouldSerializeDocument()
		{
			return Document.ShouldSerialize();
		}

		private void ResetDocument()
		{
			Document.Reset();
		}

		#region События

		public event EventHandler<BeforeChangeTexEventArgs> BeforeChangeText
		{
			add { _activeView.Document.BeforeChangeText += value; }
			remove { _activeView.Document.BeforeChangeText -= value; }
		}

		public event EventHandler<SelectionChangedEventArgs> SelectionChanged
		{
			add { _activeView.SelectionChanged += value; }
			remove { _activeView.SelectionChanged -= value; }
		}

		public event EventHandler<SelectionChangingEventArgs> SelectionChanging
		{
			add { _activeView.SelectionChanging += value; }
			remove { _activeView.SelectionChanging -= value; }
		}

		// Неделаем ничего в обработчиках OnMouseXxx, так как события мыши 
		// транслируются из активного представления.

		protected override void OnMouseDown(MouseEventArgs e) { }
		protected override void OnMouseUp(MouseEventArgs e) { }
		protected override void OnMouseEnter(EventArgs e) { }
		protected override void OnMouseLeave(EventArgs e) { }
		protected override void OnMouseHover(EventArgs e) { }
		protected override void OnMouseMove(MouseEventArgs e) { }
		protected override void OnMouseWheel(MouseEventArgs e) { }
		#endregion

		private bool _doubleBuffering;

		[DefaultValue(false)]
		public bool DoubleBuffering
		{
			get { return _doubleBuffering; }
			set
			{
				_doubleBuffering = value;
				foreach (IView view in Views)
					view.DoublleBuffering = value;
			}
		}

		private bool _animateImages = true;

		[DefaultValue(true)]
		public bool AnimateImages
		{
			get { return _animateImages; }
			set
			{
				_animateImages = value;
				foreach (IView view in Views)
					view.AnimateImages = value;
			}
		}

		private bool _showTextCaret;

		[DefaultValue(true)]
		public bool ShowTextCaret
		{
			get { return _showTextCaret; }
			set { _showTextCaret = value; }
		}

		[DefaultValue(true)]
		public bool WordWrap
		{
			[DebuggerHidden]
			get { return _activeView.WordWrap; }
			[DebuggerHidden]
			set
			{
				foreach (IView view in Views)
					view.WordWrap = value;
			}
		}

        public virtual bool ShouldChangeLine(bool isExtendSelection, int line) { return true; }
        public virtual void ProcessViewKeyPress(KeyPressEventArgs e) { }

		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public IView ActiveView
		{
			get { return _activeView; }
		}

		/// <summary>
		/// Позволяет View установить себя в качестве активного представления.
		/// Вызывается View при получении фокуса.
		/// </summary>
		/// <param name="value">Активизируемое представление</param>
		private void SetActiveView(IView value)
		{
			#region Откключение событий от старого активного представления

			if (_activeView != null)
			{
				_activeView.MouseDown  -= MouseDownHandler;
				_activeView.MouseUp    -= MouseUpHandler;
				_activeView.MouseMove  -= MouseMoveHandler;
				_activeView.MouseEnter -= MouseEnterHandler;
				_activeView.MouseHover -= MouseHoverHandler;
				_activeView.MouseLeave -= MouseLeaveHandler;
				_activeView.MouseWheel -= MouseWheelHandler;
			}

			#endregion

			_activeView = value;

			#region Откключение событий к новому активному представлению

			_activeView.MouseDown  += MouseDownHandler;
			_activeView.MouseUp    += MouseUpHandler;
			_activeView.MouseMove  += MouseMoveHandler;
			_activeView.MouseEnter += MouseEnterHandler;
			_activeView.MouseHover += MouseHoverHandler;
			_activeView.MouseLeave += MouseLeaveHandler;
			_activeView.MouseWheel += MouseWheelHandler;

			#endregion
		}

		private IView _activeView;


		#region Mouse Handler-ы

		private void MouseDownHandler(object sender, MouseEventArgs e)
		{
			base.OnMouseDown(e);
		}

		private void MouseUpHandler(object sender, MouseEventArgs e)
		{
			base.OnMouseUp(e);
		}

		private void MouseMoveHandler(object sender, MouseEventArgs e)
		{
			base.OnMouseMove(e);
		}

		private void MouseEnterHandler(object sender, EventArgs e)
		{
			base.OnMouseEnter(e);
		}

		private void MouseHoverHandler(object sender, EventArgs e)
		{
			base.OnMouseHover(e);
		}

		private void MouseLeaveHandler(object sender, EventArgs e)
		{
			base.OnMouseLeave(e);
		}

		private void MouseWheelHandler(object sender, MouseEventArgs e)
		{
			base.OnMouseWheel(e);
		}
		
		#endregion

		[DefaultValue(0)]
		public int FirstVisibleRow
		{
			get { return _activeView.FirstVisibleRow; }
			set
			{
				foreach (IView view in Views)
					view.FirstVisibleRow = value;
			}
		}

		[DefaultValue(0)]
		public int FirstVisibleCol
		{
			get { return _activeView.FirstVisibleCol; }
			set
			{
				foreach (IView view in Views)
					view.FirstVisibleCol = value;
			}
		}

		[DefaultValue(10)]
		public int HorizontalScrollOffset
		{
			get { return _activeView.HorizontalScrollStep; }
			set
			{
				foreach (IView view in Views)
					view.HorizontalScrollStep = value;
			}
		}

		[DefaultValue(false)]
		public bool ShowInvisibles
		{
			get { return _activeView.ShowInvisibles; }
			set
			{
				foreach (IView view in Views)
					view.ShowInvisibles = value;
			}
		}

		/// <summary>
		/// Стиль используемый по умолчанию в представлении.
		/// Свойство возвращает значение активного представлении, а при присвоении
		/// ему значения устанавливает присваеваемый стиль всем представлениям
		/// недактора.
		/// </summary>
		public CompleteStyle DefaultStyle
		{
			get { return _activeView.DefaultStyle; }
			set
			{
				foreach (IView view in Views)
					view.DefaultStyle = value;
			}
		}

		/// <summary>
		/// Возвращает список представлений недактора.
		/// </summary>
		public IList<IView> Views
		{
			get { return _views; }
		}

		IList<IView> _views;
		SplitContainer _horizontalSplitter;

		public bool Split
		{
			get { return _horizontalSplitter != null; }
			set
			{
				if (Split == value)
					return;

				Control view;

				if (value)
				{
					// Создаем SplitContainer, второе представление и размещаем 
					// представления внутри SplitContainer-а.
					Debug.Assert(_views.Count == 1);
					_horizontalSplitter = new SplitContainer();
					_horizontalSplitter.Orientation = Orientation.Horizontal;
					_horizontalSplitter.Dock = DockStyle.Fill;
					//_horizontalSplitter.Parent = this;

					IView secondView = CreateView();

					_views = new IView[] { _views[0], secondView };

					view = ((Control)_views[0]);
					_horizontalSplitter.Panel1.Controls.Add(view);
					view.Dock = DockStyle.Fill;

					view = ((Control)_views[1]);
					_horizontalSplitter.Panel2.Controls.Add(view);
					view.Dock = DockStyle.Fill;
					_horizontalSplitter.Parent = this;
				}
				else
				{
					Debug.Assert(_views.Count == 2);

					view = ((Control)_views[0]);
					view.Parent = this;
					view.Dock = DockStyle.Fill;
					_horizontalSplitter.Parent = null;
					_horizontalSplitter.Dispose();
					_horizontalSplitter = null;
				}
			}
		}

		private void Edite_DoubleClick(object sender, EventArgs e)
		{
#if DEBUG_FontHelper
			MessageBox.Show(this, string.Format(
				"Measure avg time MS: {0:0.0000000} My: {1:0.0000000} Count: {2}",
				BreakLine.MsMeasure / BreakLine.Count,
				BreakLine.MyMeasure / BreakLine.Count,
				BreakLine.Count));
#endif
		}
	}
}
