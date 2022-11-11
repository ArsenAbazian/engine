using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class AddRailSystemForm : FormWithSceneUpdate {
		public AddRailSystemForm() {
			InitializeComponent();
		}

		RailSystemInfoControl RailSystemInfo { get { return railSystemInfoControl1; } }
        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            railSystemInfoControl1.MainForm = MainForm;
        }

        protected virtual bool IsValidForm { 
            get { 
                if( RailSystemInfo.Name == string.Empty ) {
                    MessageBox.Show(this, "Error: empty string in Name field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (RailSystemInfo.RailFileName == string.Empty) {
                    MessageBox.Show(this, "Error: empty string in RailFileName field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (RailSystemInfo.LineModelFileName == string.Empty) {
                    MessageBox.Show(this, "Error: empty string in LineModelFileName field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (RailSystemInfo.RoundModelFileName == string.Empty) {
                    MessageBox.Show(this, "Error: empty string in RoundModelFileName field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (RailSystemInfo.LineEffectFileName == string.Empty) {
                    MessageBox.Show(this, "Error: empty string in LineEffectFileName  field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (RailSystemInfo.RoundEffectFileName == string.Empty) {
                    MessageBox.Show(this, "Error: empty string in RoundEffectFileName field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }

                if (RailSystemInfo.Round2EffectFileName == string.Empty) {
                    MessageBox.Show(this, "Error: empty string in Round2EffectFileName field!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                
                return true;
            }
        }
        
        private void okButton_Click(object sender, EventArgs e) {
            if (!IsValidForm) return;

            bool res = MainForm.Scene.AddRailSystem( RailSystemInfo.RailSystemName, 
                RailSystemInfo.RailFileName, 
                RailSystemInfo.LineModelFileName,
                RailSystemInfo.RoundModelFileName,
                RailSystemInfo.LineEffectFileName, 
                RailSystemInfo.RoundEffectFileName, 
                RailSystemInfo.Round2EffectFileName);

            if( !res ) {
                MessageBox.Show(this, "Error: can't add rail system!", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            MessageBox.Show(this, "Rail system successfully added to the scene!", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
            UpdateMainForm();
        }
    }
}