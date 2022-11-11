using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor
{
	internal class StopAnimatorHelper : IDisposable
	{
		internal StopAnimatorHelper(ImageAnimatorHelper imageAnimatorHelper)
		{
			_imageAnimatorHelper = imageAnimatorHelper;
			_redraw = _imageAnimatorHelper.Redraw;
		}

		ImageAnimatorHelper _imageAnimatorHelper;
		bool _redraw;

		#region IDisposable Members

		public void Dispose()
		{
			if (_redraw)
				_imageAnimatorHelper.Redraw = true;
		}

		#endregion
}
}
