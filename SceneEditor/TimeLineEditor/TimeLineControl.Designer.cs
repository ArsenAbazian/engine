namespace sceneEditor.TimeLineEditor {
    partial class TimeLineControl {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.settingsButton = new System.Windows.Forms.Button();
            this.loopCheck = new System.Windows.Forms.CheckBox();
            this.playButton = new System.Windows.Forms.Button();
            this.pauseButton = new System.Windows.Forms.Button();
            this.stopButton = new System.Windows.Forms.Button();
            this.startButton = new System.Windows.Forms.Button();
            this.endButton = new System.Windows.Forms.Button();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.prevFrame = new System.Windows.Forms.Button();
            this.nextFrame = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.timeLineEditor1 = new sceneEditor.TimeLineEditor.TimeLineEditor();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // settingsButton
            // 
            this.settingsButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.settingsButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.settingsButton.Location = new System.Drawing.Point(653, 28);
            this.settingsButton.Name = "settingsButton";
            this.settingsButton.Size = new System.Drawing.Size(50, 23);
            this.settingsButton.TabIndex = 1;
            this.settingsButton.Text = "settings";
            this.settingsButton.UseVisualStyleBackColor = true;
            this.settingsButton.Click += new System.EventHandler(this.settingsButton_Click);
            // 
            // loopCheck
            // 
            this.loopCheck.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.loopCheck.AutoSize = true;
            this.loopCheck.Location = new System.Drawing.Point(479, 32);
            this.loopCheck.Name = "loopCheck";
            this.loopCheck.Size = new System.Drawing.Size(50, 17);
            this.loopCheck.TabIndex = 2;
            this.loopCheck.Text = "Loop";
            this.loopCheck.UseVisualStyleBackColor = true;
            // 
            // playButton
            // 
            this.playButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.playButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.playButton.Location = new System.Drawing.Point(479, 3);
            this.playButton.Name = "playButton";
            this.playButton.Size = new System.Drawing.Size(27, 23);
            this.playButton.TabIndex = 3;
            this.playButton.Text = ">";
            this.playButton.UseVisualStyleBackColor = true;
            this.playButton.Click += new System.EventHandler(this.playButton_Click);
            // 
            // pauseButton
            // 
            this.pauseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pauseButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.pauseButton.Location = new System.Drawing.Point(512, 3);
            this.pauseButton.Name = "pauseButton";
            this.pauseButton.Size = new System.Drawing.Size(27, 23);
            this.pauseButton.TabIndex = 4;
            this.pauseButton.Text = "||";
            this.pauseButton.UseVisualStyleBackColor = true;
            this.pauseButton.Click += new System.EventHandler(this.pauseButton_Click);
            // 
            // stopButton
            // 
            this.stopButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.stopButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.stopButton.Location = new System.Drawing.Point(545, 3);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(27, 23);
            this.stopButton.TabIndex = 5;
            this.stopButton.Text = "st";
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // startButton
            // 
            this.startButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.startButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.startButton.Location = new System.Drawing.Point(578, 3);
            this.startButton.Name = "startButton";
            this.startButton.Size = new System.Drawing.Size(27, 23);
            this.startButton.TabIndex = 6;
            this.startButton.Text = "<|";
            this.startButton.UseVisualStyleBackColor = true;
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // endButton
            // 
            this.endButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.endButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.endButton.Location = new System.Drawing.Point(676, 3);
            this.endButton.Name = "endButton";
            this.endButton.Size = new System.Drawing.Size(27, 23);
            this.endButton.TabIndex = 7;
            this.endButton.Text = ">|";
            this.endButton.UseVisualStyleBackColor = true;
            this.endButton.Click += new System.EventHandler(this.endButton_Click);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.numericUpDown1.Location = new System.Drawing.Point(584, 29);
            this.numericUpDown1.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(63, 20);
            this.numericUpDown1.TabIndex = 8;
            this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // prevFrame
            // 
            this.prevFrame.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.prevFrame.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.prevFrame.Location = new System.Drawing.Point(611, 3);
            this.prevFrame.Name = "prevFrame";
            this.prevFrame.Size = new System.Drawing.Size(27, 23);
            this.prevFrame.TabIndex = 9;
            this.prevFrame.Text = "<<";
            this.prevFrame.UseVisualStyleBackColor = true;
            this.prevFrame.Click += new System.EventHandler(this.prevFrame_Click);
            // 
            // nextFrame
            // 
            this.nextFrame.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nextFrame.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.nextFrame.Location = new System.Drawing.Point(643, 3);
            this.nextFrame.Name = "nextFrame";
            this.nextFrame.Size = new System.Drawing.Size(27, 23);
            this.nextFrame.TabIndex = 10;
            this.nextFrame.Text = ">>";
            this.nextFrame.UseVisualStyleBackColor = true;
            this.nextFrame.Click += new System.EventHandler(this.nextFrame_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(537, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Speed:";
            // 
            // timeLineEditor1
            // 
            this.timeLineEditor1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.timeLineEditor1.EndTime = ((long)(1000));
            this.timeLineEditor1.LabelMiliSec = ((long)(10));
            this.timeLineEditor1.Location = new System.Drawing.Point(3, 3);
            this.timeLineEditor1.Name = "timeLineEditor1";
            this.timeLineEditor1.Size = new System.Drawing.Size(470, 51);
            this.timeLineEditor1.TabIndex = 0;
            this.timeLineEditor1.Text = "timeLineEditor1";
            this.timeLineEditor1.ThumbColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(168)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.timeLineEditor1.Value = ((long)(0));
            // 
            // TimeLineControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.nextFrame);
            this.Controls.Add(this.prevFrame);
            this.Controls.Add(this.numericUpDown1);
            this.Controls.Add(this.endButton);
            this.Controls.Add(this.startButton);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.pauseButton);
            this.Controls.Add(this.playButton);
            this.Controls.Add(this.loopCheck);
            this.Controls.Add(this.settingsButton);
            this.Controls.Add(this.timeLineEditor1);
            this.Name = "TimeLineControl";
            this.Size = new System.Drawing.Size(706, 57);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TimeLineEditor timeLineEditor1;
        private System.Windows.Forms.Button settingsButton;
        private System.Windows.Forms.CheckBox loopCheck;
        private System.Windows.Forms.Button playButton;
        private System.Windows.Forms.Button pauseButton;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Button startButton;
        private System.Windows.Forms.Button endButton;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.Button prevFrame;
        private System.Windows.Forms.Button nextFrame;
        private System.Windows.Forms.Label label1;


    }
}
