using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace imageGallery {
    public partial class ColorEditorForm : Form {
        public ColorEditorForm() {
            InitializeComponent();
        }

        public ColorEditor ColorEditor { get { return colorEditor1; } }
        ColorPicker ownerEditor;
        public ColorPicker OwnerEditor {
            get { return ownerEditor; }
            set { ownerEditor = value; } 
        }
        protected override CreateParams CreateParams {
            get {
                const int WS_DLGFRAME = 0x00400000;
                CreateParams cp = base.CreateParams;
                cp.Style &= ~WS_DLGFRAME;
                if(OwnerEditor != null) {
                    cp.X = OwnerEditor.PointToScreen(new Point(OwnerEditor.Width, OwnerEditor.Height)).X - cp.Width;
                    cp.Y = OwnerEditor.PointToScreen(new Point(OwnerEditor.Width, OwnerEditor.Height)).Y;
                    cp.Height = ColorEditor.Height + 6;
                }
                return cp;
            }
        }
        protected override void WndProc(ref Message m) {
            base.WndProc(ref m);
            
            const int WM_NCACTIVATE = 0x0086;
            if (m.Msg == WM_NCACTIVATE && m.WParam == IntPtr.Zero) 
                this.Close();
        }
    }
}