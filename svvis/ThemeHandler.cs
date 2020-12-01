using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using vis1.Properties;
using System.Windows.Forms;

namespace vis1
{
    class ThemeHandler
    {
        public static string theme = Resources.ThemeDefault;

        public static System.Drawing.Color ColorBgDark = System.Drawing.ColorTranslator.FromHtml("#1e1e1e");
        public static System.Drawing.Color ColorFgDark = System.Drawing.ColorTranslator.FromHtml("#d4d4b1");

        public static System.Drawing.Color ColorBgLight = System.Drawing.ColorTranslator.FromHtml("#f0f0f0");
        public static System.Drawing.Color ColorFgLight = System.Drawing.ColorTranslator.FromHtml("#000000");

        public static void ApplyTheme(Control target)
        {
            if (theme == Resources.ThemeDark)
            {
                target.BackColor = ColorBgDark;
                target.ForeColor = ColorFgDark;
                foreach (Control component in target.Controls)
                {
                    ApplyTheme(component);
                    // Object Dependent Custom Colors
                    if (component is Button)
                    {
                        component.BackColor = System.Drawing.ColorTranslator.FromHtml("#1e1e1e");
                        component.ForeColor = System.Drawing.ColorTranslator.FromHtml("#d4d4b1");
                    }
                }
            }
            else if (theme == Resources.ThemeLight)
            {
                target.BackColor = ColorBgLight;
                target.ForeColor = ColorFgLight;
                foreach(Control component in target.Controls)
                {   // Default Object Colors
                    ApplyTheme(component);
                    if (component is Button)
                    {
                        component.BackColor = System.Drawing.ColorTranslator.FromHtml("#f0f0f0");
                        component.ForeColor = System.Drawing.ColorTranslator.FromHtml("#000000");
                    }
                }
            }
            else
            {
                theme = Resources.ThemeDefault;
                ApplyTheme(target);
            }
        }
    }
}
