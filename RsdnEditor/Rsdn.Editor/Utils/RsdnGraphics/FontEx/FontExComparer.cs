using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace Rsdn.Editor
{
	struct FontExComparer : IEqualityComparer<PartialFont>
	{
		public static FontExComparer DefaultFontExComparer =
			new FontExComparer();

		public bool Equals(PartialFont x, PartialFont y)
		{
			if ((object)x == (object)y)
				return true;

			if (y != null && y.UsedField == x.UsedField && y.Name == x.Name 
				&& y.Style == x.Style && y.Size == x.Size && y.Unit == x.Unit)
			{
				return y.Unit == x.Unit;
			}

			return false;
		}

		public int GetHashCode(PartialFont y)
		{
			return y.HashCode;
		}
	}
}
