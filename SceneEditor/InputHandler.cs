using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using classMapper;
using System.Windows.Forms;

namespace sceneEditor {

	public class InputHandler {
		MainForm form;

		bool processMouse = false;
		Point downPos = Point.Empty;
		Vector3_Mapper y = new Vector3_Mapper(0.0f, 1.0f, 0.0f);

		public InputHandler(MainForm form) {
			this.form = form;
		}

		public MainForm Form { get { return form; } }

		public virtual void OnMouseDown(MouseEventArgs e) {
			downPos = e.Location;
		}

		public virtual void OnMouseUp(MouseEventArgs e) { }

		protected virtual void RotateCamera(MouseEventArgs e) {
			Point mouseDelta = Point.Empty;
			if(processMouse) {

				mouseDelta.X = e.Location.X - downPos.X;
				mouseDelta.Y = e.Location.Y - downPos.Y;

				downPos.X = e.Location.X;
				downPos.Y = e.Location.Y;
				if(mouseDelta.X == 0 && mouseDelta.Y == 0)
					return;

				Form.Camera.rotateAroundAxis(Form.SelectionPivot, y, 0.002f * mouseDelta.X);
				Form.Camera.rotateAroundAxis(Form.SelectionPivot, Form.Camera.AxisX, 0.002f * mouseDelta.Y);

				processMouse = false;
                Form.UpdateForm();
			}
			else
				processMouse = true;
		}

		protected virtual float GetMoveValue(float val) {
			if(Control.ModifierKeys == Keys.Control) return val * 0.01f;
			else if(Control.ModifierKeys == Keys.Alt) return val * 0.001f;
			return val * 0.005f;
		}

		protected virtual void MoveCamera(MouseEventArgs e) {

			Point mouseDelta = Point.Empty;
			if(processMouse) {

				mouseDelta.X = e.Location.X - downPos.X;
				mouseDelta.Y = e.Location.Y - downPos.Y;

				downPos.X = e.Location.X;
				downPos.Y = e.Location.Y;
				if(mouseDelta.X == 0 && mouseDelta.Y == 0)
					return;

				Form.Camera.strafeLR(-GetMoveValue(mouseDelta.X));
				Form.Camera.liftUD(GetMoveValue(mouseDelta.Y));

				processMouse = false;
                Form.UpdateForm();
			}
			else
				processMouse = true;

		}

		public virtual void OnMouseMove(MouseEventArgs e) {
			if(e.Button == MouseButtons.Left && Control.ModifierKeys == Keys.Alt) RotateCamera(e);
			else if(e.Button == MouseButtons.Right) MoveCamera(e);
        }

		public virtual void OnMouseEnter(EventArgs e) { }

		public virtual void OnMouseLeave(EventArgs e) { }

		public virtual void OnKeyDown(KeyEventArgs e) { }

		public virtual void OnKeyUp(KeyEventArgs e) { }

		public virtual float GetZDelta(MouseEventArgs e) {
			float val = (float)e.Delta / 120;

			if(Control.ModifierKeys == Keys.Control) {
				val *= 10.0f;
			}
			else if(Control.ModifierKeys == Keys.Alt) {
				val *= 0.1f;
			}
			return val;
		}

		public virtual void OnMouseWheel(MouseEventArgs e) {
			Form.Camera.moveByCameraZ(GetZDelta(e));
            Form.UpdateForm();
        }
	}
}
