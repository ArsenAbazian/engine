using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Controls {
    public partial class VectorControl : ControlWithSceneUpdate {
        private static readonly object vectorChanged = new object();
        public VectorControl() {
            InitializeComponent();
        }

        int vectorSize = 3;
        [DefaultValue(3)]
        public int VectorSize {
            get { return vectorSize; }
            set {
                if (VectorSize == value) return;
                vectorSize = value;
                OnVectorSizeChanged();
            }
        }

        protected virtual void OnVectorSizeChanged() { 
            if(VectorSize == 4) {
                v3.Visible = true;
                label4.Visible = true;
                MinimumSize = new Size(271, 42);
                MaximumSize = new Size(271, 42);
            }
            else {
                v3.Visible = false;
                label4.Visible = false;
                MinimumSize = new Size(204, 42);
                MaximumSize = new Size(204, 42);
            }
        }

        public Vector3_Mapper VectorValue3 {
            get { return new Vector3_Mapper(V0, V1, V2); }
            set { 
                V0 = value.X;
                V1 = value.Y;
                V2 = value.Z;
            }
        }

        public float V0 { get { return float.Parse(v0.Value.ToString()); } set { v0.Value = new decimal(value); } }
        public float V1 { get { return float.Parse(v0.Value.ToString()); } set { v1.Value = new decimal(value); } }
        public float V2 { get { return float.Parse(v0.Value.ToString()); } set { v2.Value = new decimal(value); } }
        public float V3 { get { return float.Parse(v0.Value.ToString()); } set { v2.Value = new decimal(value); } }

        public event EventHandler VectorChanged {
            add { Events.AddHandler(vectorChanged, value); }
            remove { Events.RemoveHandler(vectorChanged, value); }
        }

        protected virtual void RaiseVectorChanged() {
            EventHandler handler = Events[vectorChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        private void ValueChanged(object sender, EventArgs e) {
            RaiseVectorChanged();
        }
    }
}
