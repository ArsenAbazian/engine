using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Rsdn.Editor.ObjectModel;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;

namespace Rsdn.Editor
{
	internal delegate void DrawAnimatedImage(
		List<ImageInfo> animatedImagesInfo);

	class ImageAnimatorHelper : IDisposable
	{
		public ImageAnimatorHelper(DrawAnimatedImage allFrameChanged)
		{
			if (allFrameChanged == null)
				throw new ArgumentNullException("allFrameChanged");

			_control = allFrameChanged.Target as Control;

			if (_control == null)
				throw new ArgumentNullException("Delegate Target must be a Control.");

			_eventHandler = FrameChangedHandler;
			_allFrameChanged = allFrameChanged;
		}

		public StopAnimatorHelper Freeze()
		{
			return new StopAnimatorHelper(this);
		}

		private bool _redraw;

		public bool Redraw
		{
			get { return _redraw; }
			set { _redraw = value; }
		}

		Control _control;
		/// <summary>
		/// Список описаний анимированных картинок которые отображаются в данный 
		/// момент на экране.
		/// </summary>
		private List<ImageInfo> _animatedImagesDrawInfo =
			new List<ImageInfo>();
		//private int _imgPaintCounter;
		private EventHandler _eventHandler;

		private void FrameChangedHandler(object sender, EventArgs e)
		{
			if (Redraw)
				_control.BeginInvoke(AllFrameChanged, _animatedImagesDrawInfo);
		}

		public void Add(Image image, Rectangle bounds, Color backColor)
		{
			_animatedImagesDrawInfo.Add(
				new ImageInfo(image, bounds, backColor));

			ImageAnimator.Animate(image, _eventHandler);
		}

		private DrawAnimatedImage _allFrameChanged;

		public DrawAnimatedImage AllFrameChanged
		{
			get { return _allFrameChanged; }
		}

		public void Clear()
		{
#pragma warning disable 168, 219
			try
			{
				ImageInfo[] array = _animatedImagesDrawInfo.ToArray();
				_animatedImagesDrawInfo.Clear();

				foreach (ImageInfo value in array)
					ImageAnimator.StopAnimate(value.Image, _eventHandler);
			}
			catch (Exception ex)
			{
				int x = 0;
			}
#pragma warning restore
		}

		#region IDisposable Members & destructor

		~ImageAnimatorHelper()
		{
			Clear();
		}

		public void Dispose()
		{
			Clear();
			GC.SuppressFinalize(this);
		}

		#endregion
	}
}
