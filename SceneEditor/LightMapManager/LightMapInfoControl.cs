using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.LightMapManager {
    public partial class LightMapInfoControl : UserControl {
        public LightMapInfoControl() {
            InitializeComponent();
        }
    
        public bool LightMapEnabled {
            get { return enabledCheck.Checked; }
            set {
                BeginUpdate();
                enabledCheck.Checked = value;
                CancelUpdate();
            }
        }

        public string FileName {
            get { return textBox1.Text; }
            set {
                BeginUpdate();
                textBox1.Text = value;
                CancelUpdate();
            }
        }

        int[] q2Array = new int[] { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
        protected virtual void SetTextureParamEditorValue(string value) { 
            int index = 0;
            foreach(object obj in tpnCombo.Items) {
                if (obj.ToString() == value) {
                    tpnCombo.SelectedIndex = index;
                    break;
                }
                index++;
            }
            // если нашли автоматически
            if(index == tpnCombo.Items.Count && tpnCombo.SelectedItem != null) {
                LightMapInfo.LightMapTextureParamName = tpnCombo.SelectedItem.ToString();
            } 
            return;
        }

        protected virtual int q2SizeToIndex(int size) { 
            for(int i = 0; i < q2Array.Length; i++) {
                if (q2Array[i] == size) return i;
            }
            return 0;
        }

        public string TextureParamName {
            get { return tpnCombo.Text; }
            set {
                BeginUpdate();
                SetTextureParamEditorValue(value);
                CancelUpdate();
            }
        }
        
        public int LightMapWidth { 
            get {
                if (lmWidth.SelectedIndex < q2Array.Length)
                    return q2Array[lmWidth.SelectedIndex];
                return 0;
            }
            set {
                if (lmWidth.Items.Count == 0 || value < 0) return;
                BeginUpdate();
                lmWidth.SelectedIndex = q2SizeToIndex(value);
                CancelUpdate();
            }
        }

        public int LightMapHeight {
            get {
                if (lmHeight.SelectedIndex < q2Array.Length)
                    return q2Array[lmHeight.SelectedIndex];
                return 0;
            }
            set {
                if (lmHeight.Items.Count == 0 || value < 0) return;
                BeginUpdate();
                lmHeight.SelectedIndex = q2SizeToIndex(value);
                CancelUpdate();
            }
        }

        public int TextureCoordIndex {
            get { return tcIndex.SelectedIndex; }
            set {
                if (value < 0) value = 0;
                BeginUpdate();
                tcIndex.SelectedIndex = value;
                CancelUpdate();
            }
        }

        public bool IsAtlasTexture {
            get { return atlas.Checked; }
            set {
                BeginUpdate();
                atlas.Checked = value;
                CancelUpdate();
            }
        }

        string ownerName;
        public string OwnerName { 
            get { return ownerName; }
            set { ownerName = value; }
        }

        public float U0 {
            get { return float.Parse(u0.Text); }
            set {
                BeginUpdate();
                u0.Text = value.ToString();
                CancelUpdate();
            }
        }

        public float V0 {
            get { return float.Parse(v0.Text); }
            set {
                BeginUpdate();
                v0.Text = value.ToString();
                CancelUpdate();
            }
        }

        public float USize {
            get { return float.Parse(uSize.Text); }
            set {
                BeginUpdate();
                uSize.Text = value.ToString();
                CancelUpdate();
            }
        }

        public float VSize {
            get { return float.Parse(vSize.Text); }
            set {
                BeginUpdate();
                vSize.Text = value.ToString();
                CancelUpdate();
            }
        }

        a3dLightMapInfo_Mapper lightMapInfo;
        public a3dLightMapInfo_Mapper LightMapInfo {
            get { return lightMapInfo; }
            set { 
                lightMapInfo = value;
                OnLightMapInfoChanged();
            }
        }

        protected virtual void TryReplaceFile(string f1, string f2) {
            if (System.IO.File.Exists(f1)) {
                System.IO.File.Replace(f1, f2, "");
            }
        }

        private void fnButton_Click(object sender, EventArgs e) {
            if(fnDialog.ShowDialog() == DialogResult.OK) {
                TryReplaceFile(textBox1.Text, fnDialog.FileName);
                textBox1.Text = fnDialog.FileName;
            }
            OnLightMapPropertiesChanged();
        }

        private void autoFileName_Click(object sender, EventArgs e) {
            int slashIndex = FileName.LastIndexOf('\\');
            int dotIndex = FileName.LastIndexOf('.') + 1;
            if(slashIndex < 0 ) {
                MessageBox.Show("Error: select file destination first");
                return;
            }
            string fn = FileName.Substring(0, slashIndex);
            string ext = dotIndex == 0? "": "." + FileName.Substring(dotIndex, FileName.Length - dotIndex);
            fn += LightMapInfo.AutoFileName + ext;
            
            TryReplaceFile(FileName, fn);
            FileName = fn;
            OnLightMapPropertiesChanged();
        }

        protected virtual void InitializeTextureParamCombo() {
            this.tpnCombo.Items.Clear();
            foreach (string str in LightMapInfo.TextureParamColl) {
                this.tpnCombo.Items.Add(str);
            }
            SetTextureParamNameByDefault();
        }

        protected virtual void SetTextureParamNameByDefault() {
            int index = 0;
            BeginUpdate();
            foreach (object obj in tpnCombo.Items) {
                if (obj.ToString().ToUpper().Contains("LIGHTMAP")) {
                    tpnCombo.SelectedIndex = index;
                    break;
                }
                index++;
            }
            CancelUpdate();
            return;
        }

        protected virtual void OnLightMapInfoChanged() {
            if (LightMapInfo == null) {
                Visible = false;
                return; 
            }

            InitializeTextureParamCombo();
            Visible = true;
            LightMapEnabled = LightMapInfo.Enabled;
            FileName = LightMapInfo.LightMapFileName;
            TextureParamName = LightMapInfo.LightMapTextureParamName;
            LightMapWidth = LightMapInfo.LightMapWidth;
            LightMapHeight = LightMapInfo.LightMapHeight;
            TextureCoordIndex = LightMapInfo.TextureCoorIndex - 1;
            IsAtlasTexture = LightMapInfo.IsAtlasTextureCoords;
            U0 = LightMapInfo.U0;
            V0 = LightMapInfo.V0;
            USize = LightMapInfo.USize;
            VSize = LightMapInfo.VSize;
        }
        
        protected virtual void OnLightMapPropertiesChanged() {
            if (LightMapInfo == null) return;

            LightMapInfo.Enabled = LightMapEnabled;
            LightMapInfo.LightMapFileName = FileName;
            LightMapInfo.LightMapTextureParamName = TextureParamName;
            LightMapInfo.LightMapWidth = LightMapWidth;
            LightMapInfo.LightMapHeight = LightMapHeight;
            LightMapInfo.TextureCoorIndex = TextureCoordIndex + 1;
            LightMapInfo.IsAtlasTextureCoords = IsAtlasTexture;
            LightMapInfo.U0 = U0;
            LightMapInfo.V0 = V0;
            LightMapInfo.USize = USize;
            LightMapInfo.VSize = VSize;
        }
        
        int lockPropertiesChanged = 0;
        public void BeginUpdate() {
            lockPropertiesChanged++;
        }
        public void CancelUpdate() {
            if (lockPropertiesChanged == 0) return;
            lockPropertiesChanged--;
        }
        public void EndUpdate() {
            if(lockPropertiesChanged == 0) return;
            lockPropertiesChanged--;
            if (lockPropertiesChanged == 0)
                OnLightMapPropertiesChanged();
        }
        public bool IsLockUpdate { get { return lockPropertiesChanged > 0; } }
        private void enabledCheck_CheckedChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void tpnCombo_SelectedIndexChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void lmWidth_SelectedIndexChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void lmHeight_SelectedIndexChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void tcIndex_SelectedIndexChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void atlas_CheckedChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void u0_TextChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void v0_TextChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void uSize_TextChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }

        private void vSize_TextChanged(object sender, EventArgs e) {
            if (IsLockUpdate) return;
            OnLightMapPropertiesChanged();
        }
    }
}
