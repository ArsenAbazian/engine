using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Diagnostics;
using Rsdn.Editor.Formatter;
using System.Diagnostics.CodeAnalysis;

namespace Rsdn.Editor
{
	public delegate Image DisplayImage(string text);

	public abstract class Style
	{
		[DebuggerHidden]
		protected Style(string name, int hashCode)
		{
			_hashCode = hashCode;
			_name = name;
		}

		static Style() {
			
		}

        static CompleteStyle CreateDefaultStyle() {
            return new CompleteStyle("Default",
                CompleteFont.CreateFont("Arial", 16),
                KnownColor.DarkGoldenrod, KnownColor.Beige);
        }

        static CompleteStyle defaultStyle;
        public static CompleteStyle DefaultStyle {
            get {
                if (defaultStyle == null) defaultStyle = CreateDefaultStyle();
                return defaultStyle; 
            }
            set { defaultStyle = value; }
        }

		#region StyleType property

		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected StyleType _styleType;

		public StyleType StyleType
		{
			[DebuggerHidden]
			get { return _styleType; }
		}
		
		#endregion

		#region GetDisplayImage

		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected DisplayImage _displayImageHandler;

		public DisplayImage DisplayImageHandler
		{
			[DebuggerHidden]
			get { return _displayImageHandler; }
		}

		#endregion

		#region Name property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		private string _name;

		public string Name
		{
			[DebuggerHidden]
			get { return _name; }
		}

		#endregion

		#region HashCode

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		private int _hashCode;

		internal int HashCode
		{
			[DebuggerHidden]
			get { return _hashCode; }
		}

		public override int GetHashCode()
		{
			return _hashCode;
		}

		#endregion

		#region Font property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected PartialFont _font;

		public PartialFont Font
		{
			[DebuggerHidden]
			get { return _font; }
		}

		#endregion

		#region ForeColor property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected Color _foreColor;

		public Color ForeColor
		{
			[DebuggerHidden]
			get { return _foreColor; }
		}

		#endregion

		#region BackColor property

		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		protected Color _backColor;

		public Color BackColor
		{
			[DebuggerHidden]
			get { return _backColor; }
		}

		#endregion

		#region InactiveForeColor property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected Color _inactiveForeColor;

		public Color InactiveForeColor
		{
			[DebuggerHidden]
			get
			{
				return _inactiveForeColor == Color.Empty
					? _foreColor : _inactiveForeColor;
			}
		}

		#endregion

		#region InactiveBackColor property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected Color _inactiveBackColor;


		public Color InactiveBackColor
		{
			[DebuggerHidden]
			get
			{
				return _inactiveBackColor == Color.Empty 
					? _backColor : _inactiveBackColor;
			}
		}

		#endregion

		#region MarkColor property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		protected Color _markColor;

		public Color MarkColor
		{
			[DebuggerHidden]
			get { return _markColor; }
		}

		#endregion

		[DebuggerHidden]
		public override string ToString()
		{
			return Name;
		}
	}
}