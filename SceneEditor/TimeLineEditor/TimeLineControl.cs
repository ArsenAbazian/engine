using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace sceneEditor.TimeLineEditor {
    public partial class TimeLineControl : ControlWithSceneUpdate {
        Timer timer;
        
        public TimeLineControl() {
            InitializeComponent();
        }

        protected virtual Timer CreateTimer() {
            Timer tm = new Timer();
            tm.Interval = 1;
            tm.Tick += new EventHandler(OnTimerTick);
            return tm;
        }

        protected virtual long GetEllapsedMilliseconds() {
            long val = 0;
            if (Speed == 0) {
                val = Watch.ElapsedMilliseconds;
            }
            if (Speed > 0) {
                val = Watch.ElapsedMilliseconds * Speed;
            }
            if (Speed < 0) {
                val = Watch.ElapsedMilliseconds / -Speed;
                if (val == 0)
                    return val;
            }
            Watch.Reset();
            Watch.Start();
            return val;
        }

        void OnTimerTick(object sender, EventArgs e) {
            if (loopCheck.Checked && timeLineEditor1.Value == timeLineEditor1.EndTime) {
                timeLineEditor1.Value = 0;
            }
            else
                timeLineEditor1.Value += GetEllapsedMilliseconds();
        }
        
        protected Timer Timer {
            get {
                if (timer == null)
                    timer = CreateTimer();
                return timer;
            }
        }
        Stopwatch watch;
        protected Stopwatch Watch { 
            get {
                if (watch == null) watch = new Stopwatch();
                return watch;
            } 
        }

        public virtual void StartTimer() {
            if(Timer.Enabled)
                Timer.Stop();
            Timer.Start();
            Watch.Start();
        }

        public virtual void StopTimer() {
            Timer.Stop();
            timeLineEditor1.Value = 0;
            Watch.Reset();
        }

        public virtual void PauseTimer() {
            if (Watch.IsRunning) {
                Timer.Stop();
                Watch.Reset();
            }
            else {
                Timer.Start();
                Watch.Start();
            }
        }

        public virtual void NextFrame() {
            timeLineEditor1.Value++;
        }

        public virtual void PrevFrame() {
            timeLineEditor1.Value--;
        }

        public event EventHandler TimeChanged {
            add { timeLineEditor1.ValueChanged += value; }
            remove { timeLineEditor1.ValueChanged -= value; }
        }

        public TimeLineEditor Editor { get { return timeLineEditor1; } }
        public long Time { get { return timeLineEditor1.Value; } }

        private void startButton_Click(object sender, EventArgs e) {
            timeLineEditor1.Value = 0;
        }

        private void endButton_Click(object sender, EventArgs e) {
            timeLineEditor1.Value = timeLineEditor1.EndTime;
        }

        private void playButton_Click(object sender, EventArgs e) {
            StartTimer();
        }

        private void pauseButton_Click(object sender, EventArgs e) {
            PauseTimer();
        }

        private void stopButton_Click(object sender, EventArgs e) {
            StopTimer();
        }

        private void prevFrame_Click(object sender, EventArgs e) {
            PrevFrame();
        }

        private void nextFrame_Click(object sender, EventArgs e) {
            NextFrame();
        }

        int speed = 0;
        public int Speed { get { return speed; } }
        private void numericUpDown1_ValueChanged(object sender, EventArgs e) {
            speed = (int)numericUpDown1.Value;
        }

        private void settingsButton_Click(object sender, EventArgs e) {
            using (TimeLineSettingsForm form = new TimeLineSettingsForm()) {
                form.TimeLineEditor = timeLineEditor1;
                form.ShowDialog();
            }
        }
    }
}
