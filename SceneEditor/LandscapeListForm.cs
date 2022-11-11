using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class LandscapeListForm : FormWithSceneUpdate {
		public LandscapeListForm() {
			InitializeComponent();
		}

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            imageObjectListControl1.MainForm = MainForm;
            OnSceneChanged();
        }

        ImageObjectListControl LandList { get { return imageObjectListControl1; } }
        protected virtual void OnSceneChanged() {
            LandList.ImageItemList.Items.Clear();
            if (Scene == null) return;
            foreach(a3dLandscapeDesc_Mapper landDesc in Scene.Lands) {
                LandList.ImageItemList.Items.Add(new LandscapeItemInfo(landDesc));
            }
            if (LandList.ImageItemList.Items.Count > 0)
                LandList.ImageItemList.SelectedIndex = 0;
        }

        private void imageObjectListControl1_PropertiesClick(object sender, EventArgs e) {
            IItemWithImageFile item = LandList.ImageItemList.SelectedItem as IItemWithImageFile;
            LandPropForm.Landscape = item.Item as a3dLandscapeDesc_Mapper;
            LandPropForm.Show();
        }

        private void imageObjectListControl1_OkClick(object sender, EventArgs e) {
            this.Close();
        }

        LandscapePropsForm landProps;
        public LandscapePropsForm LandPropForm { 
            get { 
                if(landProps == null || landProps.IsDisposed) {
                    landProps = new LandscapePropsForm();
                    landProps.MainForm = MainForm;
                }
                return landProps;
            }
        }

        LandPassInfoForm passInfoForm;
        public LandPassInfoForm PassInfoForm { 
            get {
                if (passInfoForm == null || passInfoForm.IsDisposed)
                    passInfoForm = new LandPassInfoForm();
                return passInfoForm;
            }
        }
    }

    public class LandscapeItemInfo : IItemWithImageFile { 
        a3dLandscapeDesc_Mapper landDesc;

        public LandscapeItemInfo(a3dLandscapeDesc_Mapper landDesc) {
            this.landDesc = landDesc;
        }

        public a3dLandscapeDesc_Mapper LandDesc { get { return landDesc; } }
        
        #region IItemWithImageFile
        object IItemWithImageFile.Item { get { return LandDesc; } }
        string IItemWithImageFile.ItemName { get { return LandDesc.Name; } }
        string IItemWithImageFile.ImageFileName { get { return LandDesc.HeightMapFileName; } }
        #endregion

        public override string ToString() {
            return ((IItemWithImageFile)this).ItemName + "( " + ((IItemWithImageFile)this).ImageFileName + " )";
        }
    }
}