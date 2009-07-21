/* gmpc-easy-command.vapi generated by valac, do not modify. */

[CCode (cprefix = "Gmpc", lower_case_cprefix = "gmpc_")]
namespace Gmpc {
	[CCode (cprefix = "GmpcEasy", lower_case_cprefix = "gmpc_easy_")]
	namespace Easy {
		[CCode (cheader_filename = "gmpc-easy-command.h")]
		public class Command : Gmpc.Plugin.Base {
			[CCode (cheader_filename = "gmpc-easy-command.h")]
			public delegate void Callback (void* data, string param);
			public uint add_entry (string name, string pattern, string hint, Gmpc.Easy.Command.Callback* callback, void* userdata);
			public override bool get_enabled ();
			public override unowned string get_name ();
			public override unowned int[] get_version ();
			public static void help_window (void* data, string? param);
			public static void help_window_destroy (Gtk.Dialog window, int response);
			public Command ();
			public void popup ();
			public override void set_enabled (bool state);
		}
	}
}