// Свойства вида.

using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		#region DoublleBuffering property

		private bool _doublleBuffering;

		public bool DoublleBuffering
		{
			get { return _doublleBuffering; }
			set
			{
				_doublleBuffering = value;

				if (GetStyle(ControlStyles.OptimizedDoubleBuffer) != value)
					SetStyle(ControlStyles.OptimizedDoubleBuffer, value);
			}
		}
		
		#endregion

		#region ViewOwner property

		IViewOwner _viewOwner;

		/// <summary>
		/// Владелез данного вида. Обычно это сам редактор.
		/// </summary>
		IViewOwner IViewInternal.ViewOwner
		{
			get { return _viewOwner; }
			set { _viewOwner = value; }
		}
		
		#endregion

		#region Document property

		private Document _document;

		/// <summary>
		/// Документ отображаемый данным видом.
		/// </summary>
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public Document Document
		{
			[DebuggerHidden]
			get { return _document; }
			set
			{
				if (value == null)
					throw new ArgumentNullException("value");

				if (_document != null)
					_document.InternalViews.Remove(this);

				_document = value;
				value.InternalViews.Add(this);
				ResetViewInfo();
				this.Document.ActiveView = this;
				TextUpdated();
			}
		}
		
		#endregion

		#region Rows property

		private RowCollection _rows;

		/// <summary>
		/// Коллекция виртуальных строк вида. Ее элементы получаются в следствии
		/// преобразования осуществляемого форматером (Document.Formatter).
		/// При этом преобразовании производится перенос строк, создание тексовых
		/// регионов (Outlining) и т.п.
		/// </summary>
		public RowCollection Rows
		{
			[DebuggerHidden]
			get { return _rows; }
		}
		
		#endregion

		#region AnimateImages property

		private bool _animateImages = true;

		/// <summary>
		/// Определяет нужно ли анимировать картинки отображаемые в 
		/// картиночых-стилях.
		/// </summary>
		[DefaultValue(true)]
		[DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public bool AnimateImages
		{
			get { return _animateImages; }
			set
			{
				_animateImages = value;
				_imageAnimatorHelper.Redraw = value;
			}
		}
		
		#endregion

		#region WordWrap property

		private bool _wordWrap = true;

		/// <summary>
		/// Определяет нужно ли виду переносить, при отображении, строки документа.
		/// </summary>
		[DefaultValue(true)]
		public bool WordWrap
		{
			[DebuggerHidden]
			get { return _wordWrap; }
			[DebuggerHidden]
			set
			{
				_wordWrap = value;
				TextUpdated();
			}
		}
		
		#endregion

		#region DefaultStyle property

		/// <summary>
		/// Стиль используемый по умолчанию для вида. Берется из стиля 
		/// используемого по умолчанию для всего редактора.
		/// </summary>
		public CompleteStyle DefaultStyle
		{
			get { return _defaultStyle; }
			set
			{
				if (value == null)
					throw new ArgumentNullException("value");
			}
		}

		private CompleteStyle _defaultStyle = Style.DefaultStyle;

		#endregion

		#region ShowInvisibles property
		
		private bool _showInvisibles;

		[DefaultValue(false)]
		public bool ShowInvisibles
		{
			[DebuggerHidden]
			get { return _showInvisibles; }
			[DebuggerHidden]
			set
			{
				_showInvisibles = value;
				Invalidate();
				Update();
			}
		}

		#endregion
		
		#region TextArea (свойства связанные с текстовой областью)

		//#region DisplayRowCount property

		//public int DisplayRowCount
		//{
		//  get
		//  {
		//    return this.Height / DefaultStyle.FontHelper.Height;
		//  }
		//}
		
		//#endregion

		#region FirstVisibleVirtualRow property

		private int _firstVisibleVirtualRow;

		[DefaultValue(0)]
		public int FirstVisibleRow
		{
			[DebuggerHidden]
			get { return _firstVisibleVirtualRow; }
			[DebuggerHidden]
			set
			{
				if (value < 0)
					throw new ArgumentOutOfRangeException("value");

				if (Rows == null || value >= Rows.Count)
					throw new ArgumentOutOfRangeException("value");

				if (value != _firstVisibleVirtualRow)
				{
					using (new CaretHideHelper(this))
					{
						_firstVisibleVirtualRow = value;

						UpdateView();
					}
				}
			}
		}
		
		#endregion

		#region FirstVisibleCol (Первая видимая в виде колонка)

		private int _firstVisibleCol;

		[DefaultValue(0)]
		public int FirstVisibleCol
		{
			[DebuggerHidden]
			get { return _firstVisibleCol; }
			set
			{
				if (value < 0)
					throw new ArgumentOutOfRangeException("value");

				if (value != _firstVisibleCol)
				{
					using (new CaretHideHelper(this))
					{
						_firstVisibleCol = value;

						UpdateView();
					}
				}
			}
		}
		
		#endregion

		#region TextAreaRectangle property

		public Rectangle TextAreaRectangle
		{
			get
			{
				Rectangle clientRect = this.ClientRectangle;
				clientRect.X += StateBarWidth;
				clientRect.Width -= StateBarWidth;
				return clientRect;
			}
		}

		#endregion

		#region StateBarWidth property

		private const int DefaultStateBarWidth = 20;

		private int _stateBarWidth = DefaultStateBarWidth;

		[DefaultValue(DefaultStateBarWidth)]
		public int StateBarWidth
		{
			[DebuggerHidden]
			get { return _stateBarWidth; }
			[DebuggerHidden]
			set { _stateBarWidth = value; }
		}
		
		#endregion

		#region TextAreaWidth property

		public int TextAreaWidth
		{
			get { return Width - StateBarWidth; }
		}
		
		#endregion		
		
		#region HorizontalScrollOffset property

		/// <summary>
		/// Горизантальная сдвижка вызванная скролингом вправо.
		/// </summary>
		public int HorizontalScrollOffset { get { return _hScrollStep * FirstVisibleCol; } }
		
		#endregion

		#region HorizontalScrollOffsetStep property

		private int _hScrollStep = 10;

		[DefaultValue(10)]
		public int HorizontalScrollStep
		{
			[DebuggerHidden]
			get { return _hScrollStep; }
			[DebuggerHidden]
			set { _hScrollStep = value; }
		}
		
		#endregion

		#region TabSize property

		private const int DefaultTabSize = 5;
		private int _tabSize = DefaultTabSize;

		[DefaultValue(DefaultTabSize)]
		public int TabSize
		{
			[DebuggerHidden]
			get { return _tabSize; }
			[DebuggerHidden]
			set { _tabSize = value; }
		}
		
		#endregion

		//#region HScrollBar property

		//private HScrollBar _hScrollBar;

		//internal HScrollBar HScrollBar
		//{
		//  get { return _hScrollBar; }
		//  set { _hScrollBar = value; }
		//}

		//#endregion

		//#region VScrollBar property

		//private VScrollBar _vScrollBar;

		//internal VScrollBar VScrollBar
		//{
		//  get { return _vScrollBar; }
		//  set { _vScrollBar = value; }
		//}
		
		//#endregion

		#endregion
	}
}
