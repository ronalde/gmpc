/* gmpc-metadata-browser2.vapi generated by valac, do not modify. */

[CCode (cprefix = "Gmpc", lower_case_cprefix = "gmpc_")]
namespace Gmpc {
	[CCode (cprefix = "GmpcWidget", lower_case_cprefix = "gmpc_widget_")]
	namespace Widget {
		[CCode (cheader_filename = "gmpc-metadata-browser2.h")]
		public class More : Gtk.Frame {
			public More (string unique_id, string markup, Gtk.Widget child);
		}
		[CCode (cheader_filename = "gmpc-metadata-browser2.h")]
		public class SimilarArtist : Gtk.Table {
			public SimilarArtist (Gmpc.MetadataBrowser browser, MPD.Server server, MPD.Song song);
			public Gtk.Widget new_artist_button (string artist, bool in_db);
		}
		[CCode (cheader_filename = "gmpc-metadata-browser2.h")]
		public class SimilarSongs : Gtk.Expander {
			public SimilarSongs (MPD.Song song);
		}
	}
	[CCode (cheader_filename = "gmpc-metadata-browser2.h")]
	public class MetadataBrowser : Gmpc.Plugin.Base, Gmpc.Plugin.BrowserIface, Gmpc.Plugin.PreferencesIface {
		public const int[] version;
		public MetadataBrowser ();
		public override unowned string get_name ();
		public override unowned int[] get_version ();
		public Gtk.Widget metadata_box_show_song (MPD.Song song, bool show_controls);
		public override void save_yourself ();
		public void select_browser (Gtk.TreeView? tree);
		public void set_album (string artist, string album);
		public void set_artist (string artist);
		public void set_song (MPD.Song song);
	}
	[CCode (cheader_filename = "gmpc-metadata-browser2.h")]
	public class NowPlaying : Gmpc.Plugin.Base, Gmpc.Plugin.BrowserIface {
		public const int[] version;
		public NowPlaying ();
		public override unowned string get_name ();
		public override unowned int[] get_version ();
		public override void save_yourself ();
	}
}
