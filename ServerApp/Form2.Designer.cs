namespace ServerApp
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBoxConf = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBoxNewPass = new System.Windows.Forms.TextBox();
            this.textBoxPassConf = new System.Windows.Forms.TextBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.buttonUpdPass = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Control;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Location = new System.Drawing.Point(26, 18);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(100, 16);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = "Old Password:";
            // 
            // textBoxConf
            // 
            this.textBoxConf.Location = new System.Drawing.Point(27, 42);
            this.textBoxConf.Name = "textBoxConf";
            this.textBoxConf.PasswordChar = '*';
            this.textBoxConf.Size = new System.Drawing.Size(100, 23);
            this.textBoxConf.TabIndex = 1;
            // 
            // textBox3
            // 
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Location = new System.Drawing.Point(27, 71);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(100, 16);
            this.textBox3.TabIndex = 2;
            this.textBox3.Text = "New Password:";
            // 
            // textBoxNewPass
            // 
            this.textBoxNewPass.Location = new System.Drawing.Point(26, 100);
            this.textBoxNewPass.Name = "textBoxNewPass";
            this.textBoxNewPass.PasswordChar = '*';
            this.textBoxNewPass.Size = new System.Drawing.Size(100, 23);
            this.textBoxNewPass.TabIndex = 3;
            // 
            // textBoxPassConf
            // 
            this.textBoxPassConf.Location = new System.Drawing.Point(25, 158);
            this.textBoxPassConf.Name = "textBoxPassConf";
            this.textBoxPassConf.PasswordChar = '*';
            this.textBoxPassConf.Size = new System.Drawing.Size(100, 23);
            this.textBoxPassConf.TabIndex = 5;
            // 
            // textBox6
            // 
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox6.Location = new System.Drawing.Point(26, 129);
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(100, 16);
            this.textBox6.TabIndex = 4;
            this.textBox6.Text = "Confirm Password:";
            // 
            // buttonUpdPass
            // 
            this.buttonUpdPass.Location = new System.Drawing.Point(159, 198);
            this.buttonUpdPass.Name = "buttonUpdPass";
            this.buttonUpdPass.Size = new System.Drawing.Size(80, 23);
            this.buttonUpdPass.TabIndex = 6;
            this.buttonUpdPass.Text = "Update Pass";
            this.buttonUpdPass.UseVisualStyleBackColor = true;
            this.buttonUpdPass.Click += new System.EventHandler(this.buttonUpdPass_Click);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(304, 300);
            this.Controls.Add(this.buttonUpdPass);
            this.Controls.Add(this.textBoxPassConf);
            this.Controls.Add(this.textBox6);
            this.Controls.Add(this.textBoxNewPass);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBoxConf);
            this.Controls.Add(this.textBox1);
            this.Name = "Form2";
            this.Text = "Form2";
            this.TopMost = true;
            //this.TransparencyKey = System.Drawing.SystemColors.Control;
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBoxConf;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBoxNewPass;
        private System.Windows.Forms.TextBox textBoxPassConf;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Button buttonUpdPass;
    }
}