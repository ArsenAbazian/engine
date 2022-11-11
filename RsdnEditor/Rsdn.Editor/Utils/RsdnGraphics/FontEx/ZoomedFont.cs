using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	public class ZoomedFont : IFont
	{
		public static ZoomedFont Create(CompleteFont font, float zoom)
		{
			ZoomedFontKey key = new ZoomedFontKey(font, zoom);
			ZoomedFont zoomedFont;

			if (!_fontMap.TryGetValue(key, out zoomedFont))
				_fontMap.Add(key, zoomedFont = new ZoomedFont(font, zoom));

			return zoomedFont;
		}

		private ZoomedFont(CompleteFont font, float zoom)
		{
			if (font.Size <= 0)
				throw new ArgumentException("font.Size должен быть больше нуля.");

			if (zoom <= 0)
				throw new ArgumentException("zoom должен быть больше нуля.");

			_name = font.Name;
			_bold = font.Bold;
			_italic = font.Italic;
			_strikeout = font.Strikeout;
			_underline = font.Underline;
			_unit = font.Unit;
			_style = font.Style;
			_size = (int)font.Size * zoom;

			_fontHelper = new FontHelper(this);
		}

		FontHelper _fontHelper;

		public FontHelper FontHelper
		{
			get { return _fontHelper; }
		}

		//public override bool Equals(object obj)
		//{
		//  throw new Exception("Этот метод не должен вызваться никогда!");
		//}

		//public override int GetHashCode()
		//{
		//  throw new Exception("Этот метод не должен вызваться никогда!");
		//}

		static Dictionary<ZoomedFont.ZoomedFontKey, ZoomedFont> _fontMap = 
			new Dictionary<ZoomedFont.ZoomedFontKey, ZoomedFont>(
			new ZoomedFontKey.ZoomedFontKeyComparer());

		#region ZoomedFontKey

		private struct ZoomedFontKey
		{
			public ZoomedFontKey(CompleteFont font, float zoom)
			{
				_font = font;
				_size = (int)(font.Size * zoom * 1000);
			}

			CompleteFont _font;
			int _size;

			#region IEqualityComparer<FontEx> Members

			public struct ZoomedFontKeyComparer : IEqualityComparer<ZoomedFontKey>
			{
				public bool Equals(ZoomedFontKey x, ZoomedFontKey y)
				{
					if (x._size != y._size)
						return false;

					CompleteFont font1 = x._font;
					CompleteFont font2 = y._font;

					if ((object)font1 == (object)font2)
						return true;

					if (font1 != null && font1.UsedField == font2.UsedField
						&& font1.Name == font2.Name && font1.Style == font2.Style
						&& font1.Unit == font2.Unit)
					{
						return true;
					}

					return false;
				}

				public int GetHashCode(ZoomedFontKey obj)
				{
					return obj._font.HashCodeWithoutSize ^ obj._size;
				}
			}

			#endregion
		}
 
		#endregion

		#region IFont Members

		public string Name
		{
			get { return _name; }
		}

		private string _name;


		public bool Bold
		{
			get { return _bold; }
		}

		private bool _bold;

		public bool Italic
		{
			get { return _italic; }
		}

		bool _italic;

		public float Size
		{
			get { return _size; }
		}

		float _size;

		public bool Strikeout
		{
			get { return _strikeout; }
		}

		bool _strikeout;

		public FontStyle Style
		{
			get { return _style; }
		}

		FontStyle _style;

		public bool Underline
		{
			get { return _underline; }
		}

		bool _underline;

		public GraphicsUnit Unit
		{
			get { return _unit; }
		}

		GraphicsUnit _unit;

		#endregion

		public Font ToFont()
		{
			return new Font(Name, Size, Style, Unit);
		}
	}
}
