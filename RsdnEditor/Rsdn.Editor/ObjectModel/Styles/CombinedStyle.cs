using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Drawing;
using System.Diagnostics;

namespace Rsdn.Editor
{
	public class CombinedStyle : Style
	{
		/// <summary>
		/// Стиль комбинирующий другие стили.
		/// </summary>
		/// <param name="style1">Первый комбинируемый стиль.</param>
		/// <param name="style2">
		/// Второй комбинируемый стиль. Должен быть CombinedStyle.</param>
		/// <returns></returns>
		public static CombinedStyle Combine(Style style1, Style style2)
		{
			HashHelper hashHelper = new HashHelper(style1, style2);

			CombinedStyle cachedStyle;
			if (_styleCache.TryGetValue(hashHelper, out cachedStyle))
				return cachedStyle;

			CombinedStyle combinedStyle =
				new CombinedStyle(hashHelper.HashCode, style1, style2);

			_styleCache.Add(hashHelper, combinedStyle);

			return combinedStyle;
		}

		#region HashHelper

		private struct HashHelper
		{
			public HashHelper(Style style1, Style style2)
			{
				_hashCode = style1.HashCode + style2.HashCode;

#if DEBUG
				int bucked = _hashCode % 64;
#endif

				_style1 = style1;
				_style2 = style2;
			}

			private Style _style1;
			private Style _style2;

			[DebuggerBrowsable(DebuggerBrowsableState.Never)]
			private int _hashCode;

			public int HashCode
			{
				get { return _hashCode; }
			}

			public sealed class Comparer : IEqualityComparer<HashHelper>
			{
				#region IEqualityComparer<HashHelper> Members

				public bool Equals(HashHelper x, HashHelper y)
				{
					return object.ReferenceEquals(x._style1, y._style1)
						&& object.ReferenceEquals(x._style2, y._style2);
				}

				public int GetHashCode(HashHelper obj)
				{
					return obj._hashCode;
				}

				#endregion
			}
		}
 
		#endregion

		/// <summary>
		/// Набор закэшированных комбинированных стилей.
		/// </summary>
		private static Dictionary<HashHelper, CombinedStyle> _styleCache =
			new Dictionary<HashHelper, CombinedStyle>(64, new HashHelper.Comparer());

		/// <param name="name">Имя нового стиля.</param>
		/// <param name="hash">Хэш нового стиля.</param>
		/// <param name="style1">Первый комбинируемый стиль.</param>
		/// <param name="style2">Второй комбинируемый стиль.</param>
		/// <remarks>
		/// Комбинируемые стили кэшируются с целью повышения производильности.
		/// По этому няльзя позволять создавать их вручную. Для их создания
		/// используются статический метод Combine.
		/// </remarks>
		protected CombinedStyle(int hash, Style style1, Style style2)
			: base(style1.Name + "|" + style2.Name, hash)
		{
			bool isSelection = style1.StyleType == StyleType.Selection 
				|| style2.StyleType == StyleType.Selection;

			if (isSelection)
			{
				_styleType = StyleType.Selection;
				_foreColor = Color.FromKnownColor(KnownColor.HighlightText);
				_backColor = Color.FromKnownColor(KnownColor.Highlight);
				_inactiveForeColor = Color.FromKnownColor(KnownColor.InactiveCaptionText);
				_inactiveBackColor = Color.FromKnownColor(KnownColor.InactiveCaption);

			}
			else
			{
				_foreColor = style1.ForeColor == Color.Empty
					? style2.ForeColor : style1.ForeColor;

				_backColor = style1.BackColor == Color.Empty
					? style2.BackColor : style1.BackColor;
				
				_inactiveForeColor = style1.InactiveForeColor == Color.Empty
					? style2.InactiveForeColor : style1.InactiveForeColor;
				
				_inactiveBackColor = style1.InactiveBackColor == Color.Empty
					? style2.InactiveBackColor : style1.InactiveBackColor;
			}

			if (style1.Font == style2.Font)
				_font = style1.Font;
			else if (style1.Font == null)
				_font = style2.Font;
			else if (style2.Font == null)
				throw new ArgumentException(
					"Второй стиль обязан всегда содержать шрифт!");
			else
				_font = PartialFont.Combine(style1.Font, (CompleteFont)style2.Font);
			
			_markColor = style1.MarkColor == Color.Empty
				? style2.MarkColor : style1.MarkColor;
			
			_displayImageHandler = style1.DisplayImageHandler
				?? style2.DisplayImageHandler;
		}
	}
}