using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Drawing;
using System.Diagnostics;

namespace Rsdn.Editor
{
	/// <summary>
	/// Описывает шрийт. Не хранит никаких неуправляемых ресурсов. Каждое 
	/// описание шрифта уникально. Для получения описания пользуйтесь функцией
	/// CreateFont. Объекты не изменяемые, по этому могут безопасно 
	/// использоваться в многопоточном окружении.
	/// </summary>
	[DebuggerDisplay("{ToString()}")]
	public class PartialFont
	{
		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="fontStyle">Стили шрифта.</param>
		public static PartialFont CreateFont(FontStyle fontStyle)
		{
			return CreateFont((FontExField)fontStyle, null, fontStyle, 
				0, GraphicsUnit.Point);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="size">Размер шрифта.</param>
		/// <param name="bold">Жирный шрифт?</param>
		public static PartialFont CreateFont(float size, bool bold)
		{
			return CreateFont(null, size, bold);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		/// <returns></returns>
		public static PartialFont CreateFont(
			string name,
			float? size)
		{
			return CreateFont(name, 0, size);
		}

		public static PartialFont CreateFont(
			string name,
			float? size,
			bool? bold,
			bool? italic,
			bool? underline)
		{
			return CreateFont(name, size, bold, italic, underline, null);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		/// <param name="bold">Жирный шрифт?</param>
		/// <param name="italic">Наклонный шрифт?</param>
		/// <param name="underline">Подчеркнутый шрифт?</param>
		/// /// <param name="underline">Перечеркнутый шрифт (тоже в принципе полезно :)?</param>
		public static PartialFont CreateFont(
			string name,
			float? size,
			bool? bold,
			bool? italic,
			bool? underline,
			bool? strikeout)
		{
			FontStyle setStyleFlags = default(FontStyle);
			FontStyle clearStyleFlags = default(FontStyle);

			if (bold != null)
			{
				if (bold.Value)
					setStyleFlags |= FontStyle.Bold;
				else
					clearStyleFlags |= FontStyle.Bold;
			}

			if (italic != null)
			{
				if (italic.Value)
					setStyleFlags |= FontStyle.Italic;
				else
					clearStyleFlags |= FontStyle.Italic;
			}

			if (underline != null)
			{
				if (underline.Value)
					setStyleFlags |= FontStyle.Underline;
				else
					clearStyleFlags |= FontStyle.Underline;
			}


			if (strikeout != null)
			{
				if (strikeout.Value)
					setStyleFlags |= FontStyle.Strikeout;
				else
					clearStyleFlags |= FontStyle.Strikeout;
			}

			return CreateFont(name, setStyleFlags, clearStyleFlags, size);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		/// <param name="bold">Жирный шрифт?</param>
		/// <param name="italic">Наклонный шрифт?</param>
		public static PartialFont CreateFont(
			string name,
			float? size,
			bool? bold,
			bool? italic)
		{
			return CreateFont(name, size, bold, italic, null);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		/// <param name="bold">Жирный шрифт?</param>
		public static PartialFont CreateFont(
			string name,
			float? size,
			bool? bold)
		{
			return CreateFont(name, size, bold, null, null);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="setStyleFlags">Устанавливаемые стили шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		public static PartialFont CreateFont(
			string name,
			FontStyle setStyleFlags,
			float? size)
		{
			return CreateFont(name, setStyleFlags, 0, size);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="setStyleFlags">Устанавливаемые стили шрифта.</param>
		/// <param name="clearStyleFlags">Снимаемые стили шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		public static PartialFont CreateFont(
			string name,
			FontStyle setStyleFlags,
			FontStyle clearStyleFlags,
			float? size)
		{
			return CreateFont(name, setStyleFlags, clearStyleFlags, size, 
				GraphicsUnit.Point);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="setStyleFlags">Задаваемые стили шрифта.</param>
		/// <param name="clearStyleFlags">Снимаемые стили шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		/// <param name="unit">Еденица измерения размера шрифта.</param>
		/// <returns></returns>
		public static PartialFont CreateFont(
			string name,
			FontStyle setStyleFlags,
			FontStyle clearStyleFlags,
			float? size,
			GraphicsUnit? unit)
		{
			FontExField usedField = default(FontExField);

			if (name != null)
				usedField |= FontExField.Name;

			if ((setStyleFlags & clearStyleFlags) == 0)
			{
				usedField |= (FontExField)(setStyleFlags | clearStyleFlags);
			}
			else
				throw new ArgumentException(
					"The setStyleFlags & clearStyleFlags can't intersect.");

			if (size != null)
				usedField |= FontExField.Size;
			else if (unit != null)
				throw new ArgumentException(
					"If the the size is null unit also must be null.");

			return CreateFont(usedField, name, setStyleFlags, size ?? 0, unit ?? 0);
		}

		/// <summary>
		/// Создает шрифт по его описанию. Рельно шрифт для некоторого описания 
		/// создается только один раз. Второй раз возвращается кэшированный объект.
		/// </summary>
		/// <param name="usedField">Используемые поля.</param>
		/// <param name="name">Имя шрифта.</param>
		/// <param name="style">Стили шрифта.</param>
		/// <param name="size">Размер шрифта.</param>
		/// <param name="unit">Еденица измерения размера шрифта.</param>
		/// <returns></returns>
		public static PartialFont CreateFont(
			FontExField usedField, 
			string name,
			FontStyle style,
			float size,
			GraphicsUnit unit)
		{
			lock (_fonts)
			{
				PartialFont newFont = new PartialFont(usedField, name, style, size, unit);
				PartialFont font;

				if (_fonts.TryGetValue(newFont, out font))
					return font;

				_fonts.Add(newFont, newFont);
				return newFont;
			}
		}

		protected static Dictionary<PartialFont, PartialFont> _fonts = 
			new Dictionary<PartialFont, PartialFont>(
			FontExComparer.DefaultFontExComparer);

		protected PartialFont(
			FontExField usedField,
			string name,
			FontStyle style,
			float size,
			GraphicsUnit unit)
		{
			_usedField = usedField;

			_hashCode = (int)usedField << 23;

			if ((style & (FontStyle)0x0F) == 0 && style != 0)
				throw new ArithmeticException("Style mast be a FontStyle.");

			_style = style & (FontStyle)usedField;
			_hashCode ^= (int)_style << 19;

			if (IsUsed(usedField, FontExField.Name))
			{
				if (string.IsNullOrEmpty(name))
					throw new ArgumentNullException("name",
						"Name is null and used field mask for name not clear.");

				_name = name;
				_hashCode ^= name.GetHashCode();
			}

			_hashCodeWithoutSize = _hashCode;

			if (IsUsed(usedField, FontExField.Size))
			{
				if (size <= 0)
					throw new ArgumentNullException("size",
						"Size is 0 and used field mask for size not clear.");

				_size = size;
				_unit = unit;
				_hashCode ^= (int)((float)unit * size) << 16;
				_hashCodeWithoutSize ^= (int)unit;
			}
		}

		public static bool IsUsed(FontExField field, FontExField value)
		{
			return (field & value) == value;
		}


		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		protected FontExField _usedField;

		public FontExField UsedField
		{
			[DebuggerStepThrough]
			get { return _usedField; }
			[DebuggerStepThrough]
			protected internal set { _usedField = value; }
		}

		/// <summary>
		/// Gets style information for this System.Drawing.Font.
		/// Returns: A System.Drawing.FontStyle enumeration that contains style 
		/// information for this System.Drawing.Font.
		/// </summary>
		[Browsable(false)]
		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		public FontStyle Style { [DebuggerStepThrough] get { return _style; } }

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		protected FontStyle _style;

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font is bold.
		/// Returns true if this System.Drawing.Font is bold; otherwise, false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public bool? Bold
		{
			[DebuggerStepThrough] get { return GetState(FontStyle.Bold); }
		}

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font is italic.
		/// Returns: true if this System.Drawing.Font is italic; otherwise, false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public bool? Italic
		{
			[DebuggerStepThrough] get { return GetState(FontStyle.Italic); }
		}

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font specifies 
		/// a horizontal line through the font.
		/// Returns: true if this System.Drawing.Font has a horizontal line through
		/// it; otherwise, false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public bool? Strikeout
		{
			[DebuggerStepThrough] get { return GetState(FontStyle.Strikeout); }
		}

		/// <summary>
		/// Gets a value that indicates whether this System.Drawing.Font is 
		/// underlined.
		/// Returns: true if this System.Drawing.Font is underlined; otherwise, 
		/// false.
		/// </summary>
		[DesignerSerializationVisibility(0)]
		public bool? Underline
		{
			[DebuggerStepThrough] get { return GetState(FontStyle.Underline); }
		}

		/// <summary>
		/// Gets the face name of this System.Drawing.Font.
		/// Returns: A string representation of the face name of this System.Drawing.Font.
		/// </summary>
		[TypeConverter(typeof(FontConverter.FontNameConverter))]
		[DesignerSerializationVisibility(0)]
		//[Editor("System.Drawing.Design.FontNameEditor, System.Drawing.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", typeof(UITypeEditor))]
		public string Name { [DebuggerStepThrough] get { return _name; } }

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		private string _name;

		/// <summary>
		/// Gets the em-size of this System.Drawing.Font measured in the units 
		/// specified by the System.Drawing.Font.Unit property.
		/// Returns: The em-size of this System.Drawing.Font.
		/// </summary>
		public float? Size
		{
			[DebuggerStepThrough]
			get { return (UsedField & FontExField.Size) == 0 ? (float?)null : _size; }
		}

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		protected float _size;

		/// <summary>
		/// Gets the unit of measure for this System.Drawing.Font.
		/// Returns: A System.Drawing.GraphicsUnit that represents the unit of 
		/// measure for this System.Drawing.Font.
		/// </summary>
		[TypeConverter(typeof(FontConverter.FontUnitConverter))]
		public GraphicsUnit? Unit
		{
			[DebuggerStepThrough]
			get
			{
				return (UsedField & FontExField.Size) == 0 
					? (GraphicsUnit?)null : _unit;
			}
		}

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		protected GraphicsUnit _unit;

		/// <summary>
		/// Creates an exact copy of this System.Drawing.Font.
		/// </summary>
		/// <returns>The System.Drawing.Font this method creates, cast as an 
		/// System.Object.</returns>
		public object Clone() { return this; } // это незименяемый объект!

		/// <summary>
		/// Returns a human-readable string representation of this FontEx.
		/// </summary>
		/// <returns>A string that represents this System.Drawing.Font.</returns>
		public override string ToString()
		{
			return string.Format(
				"[{0}: Name={1}, B={2}, I={3}, U={4}, S={5}, Size={6}, Units={7}]",
				GetType().Name, Name, Bold, Italic, Underline, Strikeout, Size, Unit);
		}

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		int _hashCode;

		/// <summary>
		/// Хэш-код для внутреннего значения объекта. FontEx использует стандартную
		/// реализацию GetHashCode() и Equals() (определенную в object).
		/// Это нормально так как для таждого сочетания атрибутов шрифта может 
		/// существовать только один экземпляр класса FontEx.
		/// HashCode используется для того, чтобы обеспечить эту уникальности и
		/// обеспечить высокую проихводительность за счет кэширования хэш-значения.
		/// </summary>
		internal int HashCode
		{
			get { return _hashCode; }
		}

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		int _hashCodeWithoutSize;

		internal int HashCodeWithoutSize
		{
			get { return _hashCodeWithoutSize; }
		}

		/// <summary>
		/// Говорит установлен или нет некоторый стиль.
		/// </summary>
		/// <param name="style">Стиль для проверки.</param>
		private bool? GetState(FontStyle style)
		{
			return (UsedField & (FontExField)style) == 0
				? (bool?)null : (Style & style) != 0;
		}
		
		/// <summary>
		/// Комбинирует два шрифта возвращая новый шрифт объеденяющий свойства
		/// преданных шрифтов.
		/// При комбинировании предпочитаются настройки первого.
		/// </summary>
		/// <param name="font1">Первый шрифт подлежащий кобминированию.</param>
		/// <param name="font2">Второй шрифт подлежащий кобминированию.</param>
		/// <returns>Кобинация переданных шрифтов.</returns>
		internal static CompleteFont Combine(PartialFont font1, CompleteFont font2)
		{
			return CompleteFont.CreateFont(
				font1.Name     ?? font2.Name, 
				font1.Style     | font2.Style,
				font1.Size     ?? font2.Size, 
				font1.Unit     ?? font2.Unit);
		}
	}
}
