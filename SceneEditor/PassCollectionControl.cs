using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class PassCollectionControl : ControlWithSceneUpdate {
		private static readonly object listInit = new object();
		
		public PassCollectionControl() {
			InitializeComponent();
		}

		LandscapePassInfo passInfoControl;
		public LandscapePassInfo PassInfoControl {
			get { return passInfoControl; }
			set {
				OnPassInfoControlChanging();
				passInfoControl = value;
				OnPassInfoControlChanged();
			}
		}

        a3dLandscapeDesc_Mapper land;
        public a3dLandscapeDesc_Mapper Landscape {
            get { return land; }
            set { 
                land = value;
                OnLandscapeChanged();
            }
        }
        
        public a3dLandPassInfo_MapperCollection PassColl { 
            get {
                if (Landscape == null) return null;
                return Landscape.Pass; 
            } 
        }

        protected virtual void OnLandscapeChanged() {
            OnPassCollectionChanged();                
        }

        protected virtual void OnPassCollectionChanged() { 
            PassList.Items.Clear();
            if(PassColl == null) return;
            int passIndex = 0;
            foreach(a3dLandPassInfo_Mapper landPass in PassColl) {
                if (passIndex == Landscape.PassCount) break;
                PassList.Items.Add(new PassInfo(landPass));
                PassList.SetItemChecked(PassList.Items.Count - 1, landPass.Enabled);
                passIndex++;
            }
        }

        protected virtual void OnPassInfoControlChanging() {
			if(PassInfoControl == null) return;
			PassInfoControl.PassChanged -= new EventHandler(OnPassChanged);
        }
		protected virtual void OnPassInfoControlChanged() {
			PassInfoControl.PassChanged += new EventHandler(OnPassChanged);
		}
		protected virtual void OnPassChanged(object sender, EventArgs e) {
            if (PassInfoControl.PassEnabled && PassList.CheckedItems.Contains(PassList.SelectedItem)) return;
            if (!PassInfoControl.PassEnabled && !PassList.CheckedItems.Contains(PassList.SelectedItem)) return;
            PassList.SetItemChecked(PassList.SelectedIndex, PassInfoControl.PassEnabled);
        }

		public CheckedListBox PassList { get { return checkedListBox1; } }
		public event EventHandler PassListInitialized {
			add { Events.AddHandler( listInit, value); }
			remove { Events.RemoveHandler(listInit, value); }
		}

        private void addButton_Click(object sender, EventArgs e) {
            if (PassList.Items.Count == 0) {
                Landscape.insertPass(0);
            }
            else if (PassList.SelectedIndex < 0) {
                Landscape.insertPass(PassList.Items.Count);
            }
            else {
                Landscape.insertPass(PassList.SelectedIndex);
            }
            OnPassCollectionChanged();
        }

        private void removeButton_Click(object sender, EventArgs e) {
            if (PassList.Items.Count == 0 || PassList.SelectedIndex < 0) return;
            Landscape.removePass(PassList.SelectedIndex);
            OnPassCollectionChanged();
        }

        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e) {

        }
	}

    public class PassInfo {
        a3dLandPassInfo_Mapper landPass;

        public PassInfo(a3dLandPassInfo_Mapper landPass) {
            this.landPass = landPass;
        }

        public a3dLandPassInfo_Mapper LandPass { get { return landPass; } set { landPass = value; } }
        public override string ToString() {
            if(LandPass == null)
                return base.ToString();
            if(LandPass.Name == string.Empty || LandPass.Name == null )
                return "pass" + LandPass.PassIndex.ToString() + " ( " + LandPass.MapFileName + " )";
            return LandPass.Name + " ( " + LandPass.MapFileName + " )";
        }
    }
}
