using System;
using Rsdn.Collections.Generic;

namespace Rsdn.Editor.ObjectModel
{
	public class IViewCollection : CollectionEx<IView>
	{
		internal IViewCollection(int capacity)
			: base(capacity)
		{
		}
	}
}
