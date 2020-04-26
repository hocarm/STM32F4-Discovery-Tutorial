using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using LibUsbDotNet;
using LibUsbDotNet.Info;
using LibUsbDotNet.Main;


namespace USB_Custom_HID_Communication_GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        #region Declaring Global Variable
        public static UsbDevice myUsbDevice, myUsbDevice_temp;
        UsbEndpointReader reader;
        UsbEndpointWriter writer;
        IAsyncResult ketthuc;
        #endregion Declaring Global Variable

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(MessageBox.Show("Are you want to exit ?","Confirmation",MessageBoxButtons.YesNo,MessageBoxIcon.Question)==DialogResult.Yes)
            {

            }else
            {
                e.Cancel = true;
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                Usb_exit();
                Application.Exit();
            }
            catch
            {

            }
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void btnconnection_Click(object sender, EventArgs e)
        {
            if(btnconnection.Text== "Connect")
            {                
                if (myUsbDevice == null)
                {
                    UsbRegDeviceList allDevices = UsbDevice.AllDevices;
                    foreach (UsbRegistry usbRegistry in allDevices)
                    {
                        if (usbRegistry.Open(out myUsbDevice))
                        {
                            txtproductname.Text = myUsbDevice.Info.ProductString;
                            txtvendorid.Text = myUsbDevice.Info.Descriptor.VendorID.ToString();
                            txtproductid.Text = myUsbDevice.Info.Descriptor.ProductID.ToString();
                            txtmanufacturer.Text = myUsbDevice.Info.ManufacturerString;
                            USB_DATA_RECEIVER_INIT();
                            btnsend.Enabled = true;
                            btnconnection.Text = "Disconnect";
                        }
                    }
                }
                if (myUsbDevice == null)
                {
                    MessageBox.Show("Device Not Found !!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                Usb_exit();
                btnconnection.Text = "Connect";
                btnsend.Enabled = false;
                txtproductname.Text = "";
                txtvendorid.Text = "";
                txtproductid.Text = "";
                txtmanufacturer.Text = "";
                txtsend.Text = "";
                txtreceive.Text = "";
                txtnumberpftranmit.Text = "";
                txtnumberofreceive.Text = "";
            }
        }
        #region USB_DATA_RECEIVER_INIT
        void USB_DATA_RECEIVER_INIT()
        {
            IUsbDevice wholeUsbDevice = myUsbDevice as IUsbDevice;
            if (!ReferenceEquals(wholeUsbDevice, null))
            {
                wholeUsbDevice.SetConfiguration(1);
                wholeUsbDevice.ClaimInterface(0);
            }
            //Open usb end point reader and writer
            reader = myUsbDevice.OpenEndpointReader(ReadEndpointID.Ep01);
            writer = myUsbDevice.OpenEndpointWriter(WriteEndpointID.Ep01);
            
            //Set Interrupt service rountie for reader complete event
            reader.DataReceived += (OnRxEndPointData);
            reader.DataReceivedEnabled = true;
        }
        #endregion USB_DATA_RECEIVER_INIT

        #region USB EXIT
        private void Usb_exit()
        {
            reader.DataReceivedEnabled = false;
            reader.DataReceived -= (OnRxEndPointData);
            this.EndInvoke(ketthuc);
            reader.Dispose();
            writer.Dispose();
            if (myUsbDevice != null)
            {
                if (myUsbDevice.IsOpen)
                {
                    IUsbDevice wholeUsbDevice = myUsbDevice as IUsbDevice;
                    if (!ReferenceEquals(wholeUsbDevice, null))
                    {
                        wholeUsbDevice.ReleaseInterface(0);
                    }
                    myUsbDevice.Close();
            
                }
                myUsbDevice = null;                
                UsbDevice.Exit();
            }
        }
        #endregion USB EXIT

        #region USB DATA RECEIVER INTERRUPT SERVICE ROUNTIE
        Action<string> UsbReceiverAction;
        private void OnRxEndPointData(object sender, EndpointDataEventArgs e)
        {
            
            UsbReceiverAction = UsbReceiverActionFunction;
            if ((myUsbDevice.IsOpen) && (reader != null))
            {
                ketthuc = this.BeginInvoke(UsbReceiverAction, (Encoding.Default.GetString(e.Buffer, 0, e.Count)));
            }
        }

        private void btnsend_Click(object sender, EventArgs e)
        {
            try
            {
                byte a = Convert.ToByte(txtsend.TextLength);
                char z = Convert.ToChar(a);
                string send_data=z.ToString() + txtsend.Text;

                int bytesWritten;
                writer.Write(Encoding.Default.GetBytes(send_data), 1000, out bytesWritten);
                txtnumberpftranmit.Text = bytesWritten.ToString();
            }
            catch(Exception  err)
            {
                MessageBox.Show("Can Not Send Data To USB Device\nDetails: "+err, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            

        }

        private void UsbReceiverActionFunction(string input)
        {
            txtreceive.Text += input;
            txtnumberofreceive.Text = input.Length.ToString();
        }
            #endregion  USB DATA RECEIVER INTERRUPT SERVICE ROUNTIE
        }
}
