using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Globalization;

namespace Rsdn.Editor
{
	class DocumentTypeConverter : ExpandableObjectConverter
	{
		/// <summary>
		/// CanConvertTo
		/// </summary>
		public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
		{
			if (destinationType == typeof(string))
				return true;
			return base.CanConvertTo(context, destinationType);
		}

		/// <summary>
		/// ConvertTo
		/// </summary>
		public override object ConvertTo(ITypeDescriptorContext context,
			CultureInfo culture, object value, Type destinationType)
		{
			if (destinationType == typeof(string))
			{
				Document document = (Document)value;
				return document.Text;
			}
			return base.ConvertTo(context, culture, value, destinationType);
		}

		/// <summary>
		/// ConvertFrom
		/// </summary>
		/// <param name="context"></param>
		/// <param name="culture"></param>
		/// <param name="value"></param>
		/// <returns></returns>
		public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
		{
			string str = value as string;
			if (str != null)
			{
				return new Document(str);
			}

			return base.ConvertFrom(context, culture, value);
		}
	}
}
