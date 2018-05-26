//
//  MainViewController.swift
//  IZA
//
//  Created by Tomas Lapsansky on 20/05/2018.
//  Copyright © 2018 Tomas Lapsansky. All rights reserved.
//

import UIKit
import CoreData

var selected: NSManagedObject = NSManagedObject.init()  // Beauty
var goals = [NSManagedObject]()

class ViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {
    
    @IBOutlet weak var goalText: UITextField!
    @IBOutlet weak var tableView: UITableView!
    
    // Creation of new goal
    func insertNewGoal() {
        
        if(goalText.text != "") {
        
            let newItem = insertCoreData(title: goalText.text!)
            
            goals.append(newItem)
            
            let indexPath = IndexPath(row: goals.count - 1, section: 0)
            
            tableView.beginUpdates()
            tableView.insertRows(at: [indexPath], with: .automatic)
            tableView.endUpdates()
            
            goalText.text = ""
            view.endEditing(true)
        }
    }
    
    // Button Click
    @IBAction func addButtonAction(_ sender: Any) {
        insertNewGoal()
    }
    
    // Number of rows
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return goals.count
    }
    
    // Return Cell
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = UITableViewCell(style: .default, reuseIdentifier: "cell")
        
        cell.textLabel?.text = goals[indexPath.row].value(forKeyPath: "title") as? String
        
        if(goals[indexPath.row].value(forKey: "complete") as! Bool) {
            cell.backgroundColor = UIColor.green
            cell.isUserInteractionEnabled = false
        }
        
        return cell
    }
    
    func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        return true
    }
    
    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        
        if editingStyle == .delete {
            deleteCoreData(goal: goals[indexPath.row])
            
            goals.remove(at: indexPath.row)
            
            tableView.beginUpdates()
            tableView.deleteRows(at: [indexPath], with: .automatic)
            tableView.endUpdates()
        }
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        
        selected = goals[indexPath.row]
        performSegue(withIdentifier: "editSegue", sender: self)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.hideKeyboardWhenTappedAround()
        
        goals = loadCoreData()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        tableView.reloadData()
    }

}
