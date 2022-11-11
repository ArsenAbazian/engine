using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class ViewControl : ControlWithSceneUpdate {
        public ViewControl() {
            InitializeComponent();
        }

        public InputHandler Handler { get { return MainForm.Handler; } }

        protected override void OnMouseDown(MouseEventArgs e) {
            base.OnMouseDown(e);
            Handler.OnMouseDown(e);
        }
        protected override void OnMouseUp(MouseEventArgs e) {
            base.OnMouseUp(e);
            Handler.OnMouseUp(e);
        }
        protected override void OnMouseEnter(EventArgs e) {
            base.OnMouseEnter(e);
            Handler.OnMouseEnter(e);
        }
        protected override void OnMouseLeave(EventArgs e) {
            base.OnMouseLeave(e);
            Handler.OnMouseLeave(e);
        }
        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            Handler.OnMouseMove(e);
        }
        protected override void OnKeyDown(KeyEventArgs e) {
            base.OnKeyDown(e);
            Handler.OnKeyDown(e);
        }
        protected override void OnKeyUp(KeyEventArgs e) {
            base.OnKeyUp(e);
            Handler.OnKeyUp(e);
        }
        protected override void OnMouseWheel(MouseEventArgs e) {
            base.OnMouseWheel(e);
            Handler.OnMouseWheel(e);
        }

        protected override void OnPaint(PaintEventArgs e) {
            if (MainForm == null || !MainForm.Kernel.IsReady) {
                base.OnPaint(e);
                return;
            }

            MainForm.Kernel.BeginRender();
            MainForm.Camera.UpdateCamera(MainForm.Kernel);
            MainForm.Grid.render();
            MainForm.Scene.Render(MainForm.Camera);
            MainForm.LightMapGrid.setCamera(MainForm.Camera);
            if(MainForm.BasisRender.Basis != null)
                MainForm.BasisRender.Render();
            if(MainForm.ShowLightMapGrid)
                MainForm.LightMapGrid.Render();
            MainForm.Kernel.EndRender();
        }

        protected override void OnResize(EventArgs e) {
            base.OnResize(e);
            Invalidate();
        }

        protected override void OnPaintBackground(PaintEventArgs e) {
            if (MainForm == null) base.OnPaintBackground(e);
        }
    }
}
