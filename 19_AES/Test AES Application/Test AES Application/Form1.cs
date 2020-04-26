using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace Test_AES_Application
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        bool read_data_done = false;
        byte[] read_data = new byte[17];
        private void txttmh1_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = (!char.IsDigit(e.KeyChar)) && (!char.IsControl(e.KeyChar));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            txttmh1.Text = txttmh2.Text = txttmh3.Text = txttmh4.Text = txttmh5.Text = txttmh6.Text = txttmh7.Text = txttmh8.Text = txttmh9.Text = txttmh10.Text = txttmh11.Text = txttmh12.Text = txttmh13.Text = txttmh14.Text = txttmh15.Text = txttmh16.Text = "";
            txtsmh1.Text = txtsmh2.Text = txtsmh3.Text = txtsmh4.Text = txtsmh5.Text = txtsmh6.Text = txtsmh7.Text = txtsmh8.Text = txtsmh9.Text = txtsmh10.Text = txtsmh11.Text = txtsmh12.Text = txtsmh13.Text = txtsmh14.Text = txtsmh15.Text = txtsmh16.Text = "";
            txtsgm1.Text = txtsgm2.Text = txtsgm3.Text = txtsgm4.Text = txtsgm5.Text = txtsgm6.Text = txtsgm7.Text = txtsgm8.Text = txtsgm9.Text = txtsgm10.Text = txtsgm11.Text = txtsgm12.Text = txtsgm13.Text = txtsgm14.Text = txtsgm15.Text = txtsgm16.Text = "";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                byte[] send_data = new byte [17];

                #region Chuẩn bị dữ liệu
                if(rdomahoa.Checked==true)
                {
                    send_data[0] = 0x11;//ma hoa
                }
                if(rdogiaima.Checked==true)
                {
                    send_data[0] = 0x22;// giai ma
                }
                send_data[1] = Convert.ToByte(txttmh1.Text);
                send_data[2] = Convert.ToByte(txttmh2.Text);
                send_data[3] = Convert.ToByte(txttmh3.Text);
                send_data[4] = Convert.ToByte(txttmh4.Text);
                send_data[5] = Convert.ToByte(txttmh5.Text);
                send_data[6] = Convert.ToByte(txttmh6.Text);
                send_data[7] = Convert.ToByte(txttmh7.Text);
                send_data[8] = Convert.ToByte(txttmh8.Text);
                send_data[9] = Convert.ToByte(txttmh9.Text);
                send_data[10] = Convert.ToByte(txttmh10.Text);
                send_data[11] = Convert.ToByte(txttmh11.Text);
                send_data[12] = Convert.ToByte(txttmh12.Text);
                send_data[13] = Convert.ToByte(txttmh13.Text);
                send_data[14] = Convert.ToByte(txttmh14.Text);
                send_data[15] = Convert.ToByte(txttmh15.Text);
                send_data[16] = Convert.ToByte(txttmh16.Text);
                #endregion Chuẩn bị dữ liệu

                #region Gửi dữ liệu và chờ kết quả trả về
                groupBox5.Enabled = false;
                for (int i=0;i<17;i++)
                {
                    serialPort1.Write(send_data, i, 1);
                }

                read_data_done = false;
                while (!read_data_done) ;
                #endregion Gửi dữ liệu và chờ kết quả trả về

                #region Xử Lý Kết Quả trả về
                if (read_data[0] == 0x11)
                {
                    #region Dữ Liệu Sau Mã Hóa
                    txtsmh1.Text = read_data[1].ToString();
                    txtsmh2.Text = read_data[2].ToString();
                    txtsmh3.Text = read_data[3].ToString();
                    txtsmh4.Text = read_data[4].ToString();
                    txtsmh5.Text = read_data[5].ToString();
                    txtsmh6.Text = read_data[6].ToString();
                    txtsmh7.Text = read_data[7].ToString();
                    txtsmh8.Text = read_data[8].ToString();
                    txtsmh9.Text = read_data[9].ToString();
                    txtsmh10.Text = read_data[10].ToString();
                    txtsmh11.Text = read_data[11].ToString();
                    txtsmh12.Text = read_data[12].ToString();
                    txtsmh13.Text = read_data[13].ToString();
                    txtsmh14.Text = read_data[14].ToString();
                    txtsmh15.Text = read_data[15].ToString();
                    txtsmh16.Text = read_data[16].ToString();
                    #endregion Dữ Liệu Sau Mã Hóa
                }
                else if (read_data[0] == 0x22)
                {
                    #region Dữ Liệu Sau Giải Mã
                    txtsgm1.Text = read_data[1].ToString();
                    txtsgm2.Text = read_data[2].ToString();
                    txtsgm3.Text = read_data[3].ToString();
                    txtsgm4.Text = read_data[4].ToString();
                    txtsgm5.Text = read_data[5].ToString();
                    txtsgm6.Text = read_data[6].ToString();
                    txtsgm7.Text = read_data[7].ToString();
                    txtsgm8.Text = read_data[8].ToString();
                    txtsgm9.Text = read_data[9].ToString();
                    txtsgm10.Text = read_data[10].ToString();
                    txtsgm11.Text = read_data[11].ToString();
                    txtsgm12.Text = read_data[12].ToString();
                    txtsgm13.Text = read_data[13].ToString();
                    txtsgm14.Text = read_data[14].ToString();
                    txtsgm15.Text = read_data[15].ToString();
                    txtsgm16.Text = read_data[16].ToString();
                    #endregion Dữ Liệu Sau Giải Mã
                }
                groupBox5.Enabled = true;
                #endregion Xử Lý Kết Quả trả về
            }
            catch
            {
                MessageBox.Show("Bạn chưa nhập dữ liệu, hoặc nhập sai định dạng !", "Lỗi", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            serialPort1.Read(read_data, 0, 17);
            read_data_done = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = "COM6";
                serialPort1.Open();
            }
            catch
            {
                MessageBox.Show("Không Thể Mở Cổng COM, Hãy Kiểm Tra Lại Tên Cổng COM", "Lỗi", MessageBoxButtons.OK, MessageBoxIcon.Error);
                System.Windows.Forms.Application.Exit();
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                serialPort1.Close();
            }
        }
    }
}
