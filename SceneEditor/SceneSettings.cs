using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class SceneSettings : ControlWithSceneUpdate {
		public SceneSettings() {
			InitializeComponent();
		}

		public string SceneDataDirectory { get { return this.dataText.Text; } set { this.dataText.Text = value; } }
		public string TextureDirectory { get { return this.textureText.Text; } set { this.textureText.Text = value; } }
		public string EffectDirectory { get { return this.effectText.Text; } set { this.effectText.Text = value; } }
		public string ModelDirectory { get { return this.modelText.Text; } set { this.modelText.Text = value; } }
		public string LandDirectory { get { return this.landText.Text; } set { this.landText.Text = value; } }
		public string ParticleDirectory { get { return this.particleText.Text; } set { this.particleText.Text = value; } }
		public string RailDirectory { get { return this.railText.Text; } set { this.railText.Text = value; } }
		public string FontDirectory { get { return this.fontText.Text; } set { this.fontText.Text = value; } }
		public string SoundDirectory { get { return this.soundText.Text; } set { this.soundText.Text = value; } }

		protected virtual void InitializeFoldersByDefault() {
			TextureDirectory = "textures";
			EffectDirectory = "effects";
			ModelDirectory = "models";
			LandDirectory = "lands";
			ParticleDirectory = "particles";
			RailDirectory = "rails";
			FontDirectory = "fonts";
			SoundDirectory = "sounds";
		}
		
		private void selDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				this.dataText.Text = folderBrowser.SelectedPath;
			}
		}

		protected virtual string ExtractSubDir(string path, string initialPath) { 
			if(!path.Contains(initialPath)) return null;
			return path.Substring(initialPath.Length, path.Length - initialPath.Length);
		}

		private void textureDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string texPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(texPath == null) {
					MessageBox.Show("Error: texture directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				} 
				this.textureText.Text = texPath;
			}
		}

		private void effectDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string effPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(effPath == null) {
					MessageBox.Show("Error: effect directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.effectText.Text = effPath;
			}
		}

		private void modelDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string modelPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(modelPath == null) {
					MessageBox.Show("Error: model directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.modelText.Text = modelPath;
			}
		}

		private void landDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string landPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(landPath == null) {
					MessageBox.Show("Error: land directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.landText.Text = landPath;
			}
		}

		private void particleDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string partPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(partPath == null) {
					MessageBox.Show("Error: part directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.particleText.Text = partPath;
			}
		}

		private void railDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string railPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(railPath == null) {
					MessageBox.Show("Error: rail directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.railText.Text = railPath;
			}
		}

		private void fontDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string fontPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(fontPath == null) {
					MessageBox.Show("Error: font directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.fontText.Text = fontPath;
			}
		}

		private void soundDirButton_Click(object sender, EventArgs e) {
			if(folderBrowser.ShowDialog() == DialogResult.OK) {
				string soundPath = ExtractSubDir(folderBrowser.SelectedPath, this.dataText.Text);
				if(soundPath == null) {
					MessageBox.Show("Error: sound directory must be in data directory.", "Scene Settings", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return;
				}
				this.soundText.Text = soundPath;
			}
		}

		private void defButton_Click(object sender, EventArgs e) {
			InitializeFoldersByDefault();
		}
	}
}
