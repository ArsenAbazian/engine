using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class RailSystemInfoControl : ControlWithSceneUpdate {
		private static readonly object railFileNameChanged = new object();
        private static readonly object lineModelFileNameChanged = new object();
        private static readonly object roundModelFileNameChanged = new object();
        private static readonly object lineEffectFileNameChanged = new object();
        private static readonly object roundEffectFileNameChanged = new object();
        private static readonly object round2EffectFileNameChanged = new object();
        
        public RailSystemInfoControl() {
			InitializeComponent();
		}

		private void rfnButton_Click(object sender, EventArgs e) {
			if(railFileDialog.ShowDialog() == DialogResult.OK) {
				this.railFileName.Text = railFileDialog.FileName;
			}
		}

		private void lmfButton_Click(object sender, EventArgs e) {
			if(modelFialog.ShowDialog() == DialogResult.OK) {
				this.lineFileName.Text = this.modelFialog.FileName;
			}
		}

		private void rmfnButton_Click(object sender, EventArgs e) {
			if(modelFialog.ShowDialog() == DialogResult.OK) {
				this.roundFileName.Text = this.modelFialog.FileName;
			} 
		}

		private void lefnButton_Click(object sender, EventArgs e) {
			if(effectDialog.ShowDialog() == DialogResult.OK) {
				this.lineEffect.Text = this.effectDialog.FileName;
			}
		}

		private void refnButton_Click(object sender, EventArgs e) {
			if(effectDialog.ShowDialog() == DialogResult.OK) {
				this.roundEffect.Text = this.effectDialog.FileName;
			}
		}

		private void r2efnButton_Click(object sender, EventArgs e) {
			if(effectDialog.ShowDialog() == DialogResult.OK) {
				this.round2Effect.Text = this.effectDialog.FileName;
			}
		}

        public string RailSystemName {
            get { return railName.Text; }
            set { railName.Text = value; }
        }

        public string RailFileName {
            get { return railFileName.Text; }
            set { railFileName.Text = value; }
        }

        public string LineModelFileName {
            get { return lineFileName.Text; }
            set { lineFileName.Text = value; }
        }

        public string RoundModelFileName {
            get { return roundFileName.Text; }
            set { roundFileName.Text = value; }
        }

        public string LineEffectFileName {
            get { return lineEffect.Text; }
            set { lineEffect.Text = value; }
        }

        public string RoundEffectFileName {
            get { return roundEffect.Text; }
            set { roundEffect.Text = value; }
        }
    
        public string Round2EffectFileName {
            get { return round2Effect.Text; }
            set { round2Effect.Text = value; }
        }
    
        public event EventHandler RailFileNameChanged {
            add { Events.AddHandler(railFileNameChanged, value); }
            remove { Events.RemoveHandler(railFileNameChanged, value); }
        }

        public event EventHandler LineModelFileNameChanged {
            add { Events.AddHandler(lineModelFileNameChanged, value); }
            remove { Events.RemoveHandler(lineModelFileNameChanged, value); }
        }

        public event EventHandler RoundModelFileNameChanged {
            add { Events.AddHandler(roundModelFileNameChanged, value); }
            remove { Events.RemoveHandler(roundModelFileNameChanged, value); }
        }

        public event EventHandler LineEffectFileNameChanged {
            add { Events.AddHandler(lineEffectFileNameChanged, value); }
            remove { Events.RemoveHandler(lineEffectFileNameChanged, value); }
        }

        public event EventHandler RoundEffectFileNameChanged {
            add { Events.AddHandler(roundEffectFileNameChanged, value); }
            remove { Events.RemoveHandler(roundEffectFileNameChanged, value); }
        }

        public event EventHandler Round2EffectFileNameChanged {
            add { Events.AddHandler(round2EffectFileNameChanged, value); }
            remove { Events.RemoveHandler(round2EffectFileNameChanged, value); }
        }
    }
}
