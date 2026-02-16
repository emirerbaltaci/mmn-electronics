import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import json
import os

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
DEF_FILE = os.path.join(ROOT_DIR, "ncom_def.json")

class MessageEditor:
    def __init__(self, root):
        self.root = root
        self.root.title("NCOM Message Editor")
        self.root.geometry("1000x800")
        
        self.definitions = {}
        self.messages = [] # List of dicts with 'file_path', 'section', 'data'
        self.sections = {} # Map section name -> info
        
        # --- UI Setup ---
        
        # Toolbar
        toolbar = ttk.Frame(self.root, padding=5)
        toolbar.pack(side=tk.TOP, fill=tk.X)
        ttk.Button(toolbar, text="Reload", command=self.load_definitions).pack(side=tk.LEFT)
        ttk.Button(toolbar, text="Save All", command=self.save_all).pack(side=tk.LEFT, padx=10)
        ttk.Button(toolbar, text="New Message", command=self.create_message).pack(side=tk.LEFT)
        
        ttk.Button(toolbar, text="Manage Sections", command=self.manage_sections).pack(side=tk.LEFT, padx=10)
        ttk.Button(toolbar, text="Delete Message", command=self.delete_message).pack(side=tk.RIGHT, padx=10)
        
        # Main Paned Window
        paned = ttk.PanedWindow(self.root, orient=tk.HORIZONTAL)
        paned.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Left: Treeview
        frame_tree = ttk.Frame(paned)
        self.tree = ttk.Treeview(frame_tree, columns=("ID", "Direction"), displaycolumns=("ID", "Direction"))
        self.tree.heading("#0", text="Message / Section")
        self.tree.heading("ID", text="ID")
        self.tree.heading("Direction", text="Direction")
        self.tree.column("#0", width=200)
        self.tree.column("ID", width=50)
        self.tree.column("Direction", width=100)
        
        scrollbar = ttk.Scrollbar(frame_tree, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        self.tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        self.tree.bind("<<TreeviewSelect>>", self.on_tree_select)
        paned.add(frame_tree, weight=1)
        
        # Right: Editor
        self.editor_frame = ttk.Frame(paned, padding=10)
        paned.add(self.editor_frame, weight=2)
        
        # Editor Widgets (Created on demand or hidden)
        self.current_msg_data = None
        self.current_msg_ref = None # Reference to object in self.messages
        
        self.setup_editor_ui()
        
        # Start
        self.load_definitions()
        
    def setup_editor_ui(self):
        # Header
        f_header = ttk.Frame(self.editor_frame)
        f_header.pack(fill=tk.X)
        
        ttk.Label(f_header, text="ID:").grid(row=0, column=0, sticky="e")
        self.var_id = tk.IntVar()
        ttk.Entry(f_header, textvariable=self.var_id, width=10).grid(row=0, column=1, sticky="w")
        
        ttk.Label(f_header, text="Name:").grid(row=0, column=2, sticky="e")
        self.var_name = tk.StringVar()
        ttk.Entry(f_header, textvariable=self.var_name, width=20).grid(row=0, column=3, sticky="w")
        
        ttk.Label(f_header, text="Direction:").grid(row=1, column=0, sticky="e")
        self.var_dir = tk.StringVar()
        self.combo_dir = ttk.Combobox(f_header, textvariable=self.var_dir, values=["MCU_TO_HOST", "HOST_TO_MCU", "BIDIRECTIONAL"])
        self.combo_dir.grid(row=1, column=1, sticky="w")
        
        ttk.Label(f_header, text="Description:").grid(row=2, column=0, sticky="ne")
        self.txt_desc = tk.Text(f_header, height=3, width=50)
        self.txt_desc.grid(row=2, column=1, columnspan=3, sticky="we")
        
        # Payload
        ttk.Label(self.editor_frame, text="Payload Fields", font=("Bold")).pack(fill=tk.X, pady=(10, 5))
        
        f_payload_list = ttk.Frame(self.editor_frame)
        f_payload_list.pack(fill=tk.BOTH, expand=True)
        
        self.tree_payload = ttk.Treeview(f_payload_list, columns=("Type", "Desc"), displaycolumns=("Type", "Desc"))
        self.tree_payload.heading("#0", text="Name")
        self.tree_payload.heading("Type", text="Type")
        self.tree_payload.heading("Desc", text="Description")
        self.tree_payload.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        # Payload Actions
        f_p_actions = ttk.Frame(self.editor_frame)
        f_p_actions.pack(fill=tk.X)
        ttk.Button(f_p_actions, text="Add Field", command=self.add_payload_field).pack(side=tk.LEFT)
        ttk.Button(f_p_actions, text="Remove Field", command=self.remove_payload_field).pack(side=tk.LEFT)
        ttk.Button(f_p_actions, text="Edit Field", command=self.edit_payload_field).pack(side=tk.LEFT)
        
        # Commit button
        ttk.Button(self.editor_frame, text="Update Message Object", command=self.commit_changes).pack(anchor="ne", pady=10)

    def load_definitions(self):
        """Full reload from disk."""
        self.load_data_from_disk()
        self.rebuild_tree()

    def load_data_from_disk(self):
        self.messages = []
        self.sections = {}
        
        try:
            with open(DEF_FILE, 'r') as f:
                self.definitions = json.load(f)
            
            # Populate sections map
            if "msg_sections" in self.definitions:
                for sec in self.definitions["msg_sections"]:
                    self.sections[sec["name"]] = sec

            # Load messages
            if "includes" in self.definitions:
                for sec_name, inc_data in self.definitions["includes"].items():
                    if not inc_data.get("enabled", False):
                        continue
                        
                    path = os.path.join(ROOT_DIR, inc_data["path"])
                    if os.path.exists(path):
                        try:
                            with open(path, 'r') as f:
                                data = json.load(f)
                                
                            if "messages" in data:
                                for msg in data["messages"]:
                                    msg_obj = {
                                        "section": sec_name,
                                        "file_path": path,
                                        "data": msg
                                    }
                                    self.messages.append(msg_obj)
                        except Exception as e:
                            print(f"Error loading {path}: {e}")
                            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load definition: {e}")

    def rebuild_tree(self):
        # Clear tree
        self.tree.delete(*self.tree.get_children())
        
        # Add Sections
        if "msg_sections" in self.definitions:
            for sec in self.definitions["msg_sections"]:
                # Ensure section is in self.sections map (in case it was added in memory)
                self.sections[sec["name"]] = sec 
                self.tree.insert("", "end", iid=sec["name"], text=f"{sec['name']} ({sec['id_range'][0]}-{sec['id_range'][1]})", open=True)

        # Add Messages
        for m in self.messages:
            sec_name = m["section"]
            msg = m["data"]
            if self.tree.exists(sec_name):
                # Check if node exists (it might be duplicate if ID reused? using msg_ID as key)
                node_id = f"msg_{msg['id']}"
                if self.tree.exists(node_id):
                    # Edge case: duplicate ID in memory? 
                    # If so, maybe append suffix or warn?
                    # For now, let's just let it be or update it. Treeview keys must be unique.
                    pass
                else:
                    self.tree.insert(sec_name, "end", iid=node_id, text=msg["name"], values=(msg["id"], msg.get("direction", "?")))

    def on_tree_select(self, event):
        sel = self.tree.selection()
        if not sel:
            return
            
        item_id = sel[0]
        if item_id.startswith("msg_"):
            # It's a message
            msg_id = int(item_id.split("_")[1])
            # Find in self.messages
            for m in self.messages:
                if m["data"]["id"] == msg_id:
                    self.load_message_to_editor(m)
                    break
        else:
            # It's a section
            pass

    def load_message_to_editor(self, msg_obj):
        self.current_msg_ref = msg_obj
        data = msg_obj["data"]
        
        self.var_id.set(data["id"])
        self.var_name.set(data["name"])
        self.var_dir.set(data.get("direction", "MCU_TO_HOST"))
        
        self.txt_desc.delete("1.0", tk.END)
        self.txt_desc.insert("1.0", data.get("desc", ""))
        
        # Load payload
        self.tree_payload.delete(*self.tree_payload.get_children())
        if "payload" in data:
            for field in data["payload"]:
                self.tree_payload.insert("", "end", text=field["name"], values=(field["type"], field.get("desc", "")))

    def commit_changes(self):
        if not self.current_msg_ref:
            return
            
        # Validate ID
        try:
            new_id = self.var_id.get()
        except Exception:
            messagebox.showerror("Error", "Invalid ID")
            return

        # Check for ID conflict (if ID changed)
        current_data = self.current_msg_ref["data"]
        if new_id != current_data["id"]:
            # Check collision
            for m in self.messages:
                if m["data"]["id"] == new_id:
                    messagebox.showerror("Error", f"ID {new_id} is already used by {m['data']['name']}")
                    return
        
        # Check ID Range for this section
        sec_name = self.current_msg_ref["section"]
        if sec_name in self.sections:
            rng = self.sections[sec_name]["id_range"]
            if not (rng[0] <= new_id <= rng[1]):
                 # Warn but allow? Or Block? Let's warn.
                 if not messagebox.askyesno("Warning", f"ID {new_id} is outside the defined range for section '{sec_name}' ({rng[0]}-{rng[1]}). Continue?"):
                     return

        # Update internal object from UI
        data = self.current_msg_ref["data"]
        data["id"] = new_id
        data["name"] = self.var_name.get()
        data["direction"] = self.var_dir.get()
        data["desc"] = self.txt_desc.get("1.0", tk.END).strip()
        
        # Rebuild payload list
        new_payload = []
        for child in self.tree_payload.get_children():
            item = self.tree_payload.item(child)
            name = item["text"]
            values = item["values"]
            
            # Find original field data to preserve extra keys (enum, flags etc)
            existing_field = next((f for f in data.get("payload", []) if f["name"] == name), None)
            
            if existing_field:
                existing_field["type"] = values[0]
                existing_field["desc"] = values[1]
                new_payload.append(existing_field)
            else:
                new_payload.append({
                    "name": name,
                    "type": values[0],
                    "desc": values[1]
                })
        
        data["payload"] = new_payload
        
        # Refresh tree and restore selection
        self.rebuild_tree()
        
        node_id = f"msg_{data['id']}"
        if self.tree.exists(node_id):
            self.tree.selection_set(node_id)
            self.tree.see(node_id)
            
        # messagebox.showinfo("Success", "Message updated in memory. Remember to Save All.")
        
    def save_all(self):
        # 1. Save ncom_def.json (Sections might have changed)
        try:
            with open(DEF_FILE, 'w') as f:
                json.dump(self.definitions, f, indent=4)
        except Exception as e:
            messagebox.showerror("Error", f"Failed to save definitions: {e}")
            return

        # 2. Save Message files
        # Group messages by file path
        files_map = {}
        
        for m in self.messages:
            path = m["file_path"]
            if path not in files_map:
                files_map[path] = {"section": m["section"], "messages": []}
            files_map[path]["messages"].append(m["data"])
            
        # Write to files
        try:
            # Iterate known files (from self.messages)
            # CAUTION: If all messages in a file were deleted, it won't be in files_map.
            # We should probably track all loaded files to handle empty files properly.
            # For now, we trust files_map, meaning empty files might not be updated if we don't track them.
            # But let's assume at least one message per file for now or user doesn't delete all.
            # PROPER FIX: Iterate over definitions['includes'] to find all files.
            
            all_paths = []
            if "includes" in self.definitions:
                for sec_name, inc_data in self.definitions["includes"].items():
                    if inc_data.get("enabled", False):
                        all_paths.append(os.path.join(ROOT_DIR, inc_data["path"]))
            
            for path in all_paths:
                content = files_map.get(path)
                
                # If content is None, it means no messages for this file (or we didn't load any)
                # We should still write an empty list if we loaded it before.
                # Need to know the section name corresponding to the path.
                
                # Find section name for this path
                section_name = None
                for name, inc in self.definitions["includes"].items():
                    if os.path.join(ROOT_DIR, inc["path"]) == path:
                        section_name = name
                        break
                
                messages_to_write = []
                if content:
                    messages_to_write = content["messages"]
                
                # Prepare data
                 # We need to read original file to preserve header/other info?
                if os.path.exists(path):
                    with open(path, 'r') as f:
                        existing_data = json.load(f)
                else:
                    # Find section info for range
                    rng = [0, 0]
                    if section_name and section_name in self.sections:
                        rng = self.sections[section_name]["id_range"]
                    
                    existing_data = {"section": section_name, "id_range": rng} # Fallback

                existing_data["messages"] = messages_to_write
                
                with open(path, 'w') as f:
                    json.dump(existing_data, f, indent=4)
                    
            messagebox.showinfo("Success", "All files saved successfully.")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to save: {e}")

    def create_message(self):
        # 1. Ask for Section
        section_names = list(self.sections.keys())
        if not section_names:
            messagebox.showerror("Error", "No sections found.")
            return
            
        # Simple Dialog to ask for section
        dialog = tk.Toplevel(self.root)
        dialog.title("Select Section")
        
        ttk.Label(dialog, text="Section:").pack(padx=10, pady=5)
        combo = ttk.Combobox(dialog, values=section_names)
        combo.pack(padx=10, pady=5)
        combo.current(0)
        
        def on_create():
            sec_name = combo.get()
            dialog.destroy()
            self._create_message_internal(sec_name)
            
        ttk.Button(dialog, text="Create", command=on_create).pack(pady=10)

    def _create_message_internal(self, section_name):
        # Determine new ID
        sec_info = self.sections.get(section_name)
        if not sec_info:
            return
            
        id_range = sec_info["id_range"]
        # Find used IDs in this section
        used_ids = [m["data"]["id"] for m in self.messages if m["section"] == section_name]
        
        new_id = id_range[0]
        while new_id in used_ids:
            new_id += 1
            
        if new_id > id_range[1]:
            messagebox.showwarning("Warning", "No free IDs in this section range!")
            # Continue anyway?
            
        new_msg_data = {
            "id": new_id,
            "name": f"NEW_MSG_{new_id}",
            "direction": "MCU_TO_HOST",
            "desc": "New message description",
            "payload": []
        }
        
        # Determine file path from ncom_def includes
        file_path = None
        if "includes" in self.definitions:
            inc = self.definitions["includes"].get(section_name)
            if inc:
                file_path = os.path.join(ROOT_DIR, inc["path"])
        
        if not file_path:
             messagebox.showerror("Error", "Could not determine file path for section.")
             return

        msg_obj = {
            "section": section_name,
            "file_path": file_path,
            "data": new_msg_data
        }
        self.messages.append(msg_obj)
        
        # Add to tree
        self.rebuild_tree()
        node_id = f"msg_{new_id}"
        if self.tree.exists(node_id):
            self.tree.selection_set(node_id)
            self.tree.see(node_id)

    def delete_message(self):
        sel = self.tree.selection()
        if not sel:
            return
            
        item_id = sel[0]
        if item_id.startswith("msg_"):
            msg_id = int(item_id.split("_")[1])
            # Find in self.messages
            for i, m in enumerate(self.messages):
                if m["data"]["id"] == msg_id:
                    # Confirm
                    if messagebox.askyesno("Confirm Delete", f"Are you sure you want to delete message {msg_id} ({m['data']['name']})?"):
                        del self.messages[i]
                        self.tree.delete(item_id)
                        # Clear editor
                        self.current_msg_ref = None
                        self.var_id.set(0)
                        self.var_name.set("")
                        self.txt_desc.delete("1.0", tk.END)
                        self.tree_payload.delete(*self.tree_payload.get_children())
                    break
    
    def manage_sections(self):
        ManageSectionsDialog(self.root, self.definitions["msg_sections"], self.on_sections_updated)

    def on_sections_updated(self, new_sections):
        self.definitions["msg_sections"] = new_sections
        self.rebuild_tree()

    def add_payload_field(self):
        AddPayloadDialog(self.root, self.on_payload_added)
        
    def on_payload_added(self, name, type_, desc):
        self.tree_payload.insert("", "end", text=name, values=(type_, desc))

    def remove_payload_field(self):
        sel = self.tree_payload.selection()
        if sel:
            self.tree_payload.delete(sel[0])

    def edit_payload_field(self):
        sel = self.tree_payload.selection()
        if sel:
            item = self.tree_payload.item(sel[0])
            # Pass existing values
            AddPayloadDialog(self.root, 
                             lambda n, t, d: self.on_payload_edited(sel[0], n, t, d),
                             initial_name=item["text"], 
                             initial_type=item["values"][0], 
                             initial_desc=item["values"][1])

    def on_payload_edited(self, item_id, name, type_, desc):
        self.tree_payload.item(item_id, text=name, values=(type_, desc))


class ManageSectionsDialog(tk.Toplevel):
    def __init__(self, parent, sections_list, callback):
        super().__init__(parent)
        self.callback = callback
        self.sections = sections_list # Reference to list of dicts
        self.title("Manage Sections")
        self.geometry("600x400")
        
        # Grid layout for editing
        self.frame = ttk.Frame(self)
        self.frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Headers
        ttk.Label(self.frame, text="Section Name").grid(row=0, column=0)
        ttk.Label(self.frame, text="Start ID").grid(row=0, column=1)
        ttk.Label(self.frame, text="End ID").grid(row=0, column=2)
        ttk.Label(self.frame, text="Description").grid(row=0, column=3)
        
        self.entries = []
        
        for i, sec in enumerate(self.sections):
            row = i + 1
            ttk.Label(self.frame, text=sec["name"]).grid(row=row, column=0, sticky="w")
            
            var_start = tk.IntVar(value=sec["id_range"][0])
            e_start = ttk.Entry(self.frame, textvariable=var_start, width=8)
            e_start.grid(row=row, column=1)
            
            var_end = tk.IntVar(value=sec["id_range"][1])
            e_end = ttk.Entry(self.frame, textvariable=var_end, width=8)
            e_end.grid(row=row, column=2)
            
            # Desc (Current readonly/label) or editable?
            ttk.Label(self.frame, text=sec.get("desc", "")).grid(row=row, column=3, sticky="w")
            
            self.entries.append({
                "name": sec["name"],
                "start": var_start,
                "end": var_end
            })
            
        ttk.Button(self, text="Save Changes", command=self.on_save).pack(pady=10)
        
    def on_save(self):
        # Update local list
        for entry in self.entries:
            # Find section
            sec = next((s for s in self.sections if s["name"] == entry["name"]), None)
            if sec:
                try:
                    start = entry["start"].get()
                    end = entry["end"].get()
                    if start > end:
                         messagebox.showerror("Error", f"Invalid range for {sec['name']}: Start > End")
                         return
                    sec["id_range"] = [start, end]
                except Exception:
                    messagebox.showerror("Error", f"Invalid input for {sec['name']}")
                    return
        
        if self.callback:
            self.callback(self.sections)
        self.destroy()

class AddPayloadDialog(tk.Toplevel):
    def __init__(self, parent, callback, initial_name="", initial_type="uint8", initial_desc=""):
        super().__init__(parent)
        self.callback = callback
        self.title("Payload Field")
        
        ttk.Label(self, text="Name:").grid(row=0, column=0, padx=5, pady=5)
        self.name_var = tk.StringVar(value=initial_name)
        ttk.Entry(self, textvariable=self.name_var).grid(row=0, column=1, padx=5, pady=5)
        
        ttk.Label(self, text="Type:").grid(row=1, column=0, padx=5, pady=5)
        self.type_var = tk.StringVar(value=initial_type)
        # Standard types
        types = ["uint8", "int8", "uint16", "int16", "uint32", "int32", "float32", "float64"] # Standardized to float32/64
        ttk.Combobox(self, textvariable=self.type_var, values=types).grid(row=1, column=1, padx=5, pady=5)
        
        ttk.Label(self, text="Desc:").grid(row=2, column=0, padx=5, pady=5)
        self.desc_var = tk.StringVar(value=initial_desc)
        ttk.Entry(self, textvariable=self.desc_var).grid(row=2, column=1, padx=5, pady=5)
        
        ttk.Button(self, text="OK", command=self.on_ok).grid(row=3, column=0, columnspan=2, pady=10)
        
    def on_ok(self):
        if self.callback:
            self.callback(self.name_var.get(), self.type_var.get(), self.desc_var.get())
        self.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = MessageEditor(root)
    root.mainloop()
