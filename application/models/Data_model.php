<?php
class Data_model extends CI_Model {
    
    public $title;
    public $content;
    public $date;
    public function __construct()
    {
        parent::__construct();
    }
    public function getData($table, $limit = 100) {
        
        $result = $this->mongo_db->limit($limit)->get($table);
        return $result;
    }
    
    public function insert_entry()
    {
            $this->title    = $_POST['title']; // please read the below note
            $this->content  = $_POST['content'];
            $this->date     = time();

            $this->db->insert('entries', $this);
    }

    public function update_entry()
    {
            $this->title    = $_POST['title'];
            $this->content  = $_POST['content'];
            $this->date     = time();

            $this->db->update('entries', $this, array('id' => $_POST['id']));
    }
}