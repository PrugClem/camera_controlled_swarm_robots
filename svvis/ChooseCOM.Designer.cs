using System.Windows.Forms;

namespace vis1
{
    partial class ChooseCom
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
            this.PortsLB = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonRefresh = new System.Windows.Forms.Button();
            this.selectBAUD = new System.Windows.Forms.ComboBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.selectDataBits = new System.Windows.Forms.ComboBox();
            this.selectParity = new System.Windows.Forms.ComboBox();
            this.selectStopBits = new System.Windows.Forms.ComboBox();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // PortsLB
            // 
            this.PortsLB.FormattingEnabled = true;
            this.PortsLB.ItemHeight = 16;
            this.PortsLB.Location = new System.Drawing.Point(16, 44);
            this.PortsLB.Margin = new System.Windows.Forms.Padding(4);
            this.PortsLB.Name = "PortsLB";
            this.PortsLB.Size = new System.Drawing.Size(100, 116);
            this.PortsLB.TabIndex = 0;
            this.PortsLB.SelectedIndexChanged += new System.EventHandler(this.PortsLB_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 11);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(258, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Wählen Sie eine COM-Schnittstelle aus:";
            // 
            // buttonOK
            // 
            this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonOK.Location = new System.Drawing.Point(16, 169);
            this.buttonOK.Margin = new System.Windows.Forms.Padding(4);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(100, 28);
            this.buttonOK.TabIndex = 2;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(232, 168);
            this.buttonCancel.Margin = new System.Windows.Forms.Padding(4);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(100, 28);
            this.buttonCancel.TabIndex = 3;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonRefresh
            // 
            this.buttonRefresh.Location = new System.Drawing.Point(124, 168);
            this.buttonRefresh.Margin = new System.Windows.Forms.Padding(4);
            this.buttonRefresh.Name = "buttonRefresh";
            this.buttonRefresh.Size = new System.Drawing.Size(100, 28);
            this.buttonRefresh.TabIndex = 4;
            this.buttonRefresh.Text = "Refresh";
            this.buttonRefresh.UseVisualStyleBackColor = true;
            this.buttonRefresh.Click += new System.EventHandler(this.buttonRefresh_Click);
            // 
            // selectBAUD
            // 
            this.selectBAUD.Items.AddRange(new object[] {
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.selectBAUD.Location = new System.Drawing.Point(232, 44);
            this.selectBAUD.Name = "selectBAUD";
            this.selectBAUD.Size = new System.Drawing.Size(100, 24);
            this.selectBAUD.TabIndex = 5;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(124, 44);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(100, 24);
            this.panel1.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(66, 17);
            this.label2.TabIndex = 0;
            this.label2.Text = "Baudrate";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.label3);
            this.panel2.Location = new System.Drawing.Point(124, 74);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(100, 24);
            this.panel2.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 3);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(64, 17);
            this.label3.TabIndex = 0;
            this.label3.Text = "Data bits";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.label4);
            this.panel3.Location = new System.Drawing.Point(124, 104);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(100, 24);
            this.panel3.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 3);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 17);
            this.label4.TabIndex = 0;
            this.label4.Text = "Parity";
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label5);
            this.panel4.Location = new System.Drawing.Point(124, 134);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(100, 24);
            this.panel4.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 3);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(63, 17);
            this.label5.TabIndex = 0;
            this.label5.Text = "Stop bits";
            // 
            // selectDataBits
            // 
            this.selectDataBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.selectDataBits.FormattingEnabled = true;
            this.selectDataBits.Items.AddRange(new object[] {
            "4",
            "5",
            "6",
            "7",
            "8"});
            this.selectDataBits.Location = new System.Drawing.Point(232, 74);
            this.selectDataBits.Name = "selectDataBits";
            this.selectDataBits.Size = new System.Drawing.Size(100, 24);
            this.selectDataBits.TabIndex = 10;
            // 
            // selectParity
            // 
            this.selectParity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.selectParity.FormattingEnabled = true;
            this.selectParity.Items.AddRange(new object[] {
            "None",
            "Odd",
            "Even",
            "Mark",
            "Space"});
            this.selectParity.Location = new System.Drawing.Point(232, 104);
            this.selectParity.Name = "selectParity";
            this.selectParity.Size = new System.Drawing.Size(100, 24);
            this.selectParity.TabIndex = 11;
            // 
            // selectStopBits
            // 
            this.selectStopBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.selectStopBits.FormattingEnabled = true;
            this.selectStopBits.Items.AddRange(new object[] {
            "One",
            "OnePointFive",
            "Two"});
            this.selectStopBits.Location = new System.Drawing.Point(232, 134);
            this.selectStopBits.Name = "selectStopBits";
            this.selectStopBits.Size = new System.Drawing.Size(100, 24);
            this.selectStopBits.TabIndex = 12;
            // 
            // ChooseCom
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(355, 210);
            this.Controls.Add(this.selectStopBits);
            this.Controls.Add(this.selectParity);
            this.Controls.Add(this.selectDataBits);
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.selectBAUD);
            this.Controls.Add(this.buttonRefresh);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.PortsLB);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "ChooseCom";
            this.Text = "Choose COM-Port";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox PortsLB;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private Button buttonRefresh;
        private ComboBox selectBAUD;
        private Panel panel1;
        private Label label2;
        private Panel panel2;
        private Label label3;
        private Panel panel3;
        private Label label4;
        private Panel panel4;
        private Label label5;
        private ComboBox selectDataBits;
        private ComboBox selectParity;
        private ComboBox selectStopBits;
    }
}