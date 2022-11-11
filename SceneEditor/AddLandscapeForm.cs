using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class AddLandscapeForm : FormWithSceneUpdate {
        public AddLandscapeForm() {
            InitializeComponent();
        }

        public LandscapeProps LandscapeProps { get { return landscapeProps1; } }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            landscapeProps1.MainForm = MainForm;
        }
        
        private void okButton_Click(object sender, EventArgs e) {
            if (!LandscapeProps.IsCorrectData)
                return;

            if( Scene == null ) {
                MessageBox.Show("Error: Landscape not loaded. Scene == null.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
                return;
            }
        
            if( !Scene.AddLandscape( LandscapeProps.LandName, 
                                    LandscapeProps.HeightMapFileName, 
                                    LandscapeProps.LightMapFileName, 
                                    LandscapeProps.EffectFileName, 
                                    LandscapeProps.PixelError,
                                    LandscapeProps.PatchPerSide, 
                                    LandscapeProps.HeightScale, 
                                    LandscapeProps.SideScale, 
                                    LandscapeProps.LightMapWidth, 
                                    LandscapeProps.LightMapHeight ) ) {
                this.Close();
                MessageBox.Show("Error: can't add landscape.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            this.Close();
            MessageBox.Show("Landscape successfully added. Add at least one pass to view landscape.", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
            return;
        }

        private void AddLandscapeForm_Load(object sender, EventArgs e) {
            
        }
    }
}