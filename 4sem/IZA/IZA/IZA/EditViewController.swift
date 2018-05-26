//
//  EditViewController.swift
//  IZA
//
//  Created by Tomas Lapsansky on 22/05/2018.
//  Copyright © 2018 Tomas Lapsansky. All rights reserved.
//

import UIKit
import Foundation

class EditViewController: UIViewController {
    
    @IBOutlet weak var titleField: UITextField!
    @IBOutlet weak var descriptionField: UITextField!
    @IBOutlet weak var alertCheck: UISwitch!
    @IBOutlet weak var completeCheck: UISwitch!
    @IBOutlet weak var alertDate: UIDatePicker!
    @IBOutlet weak var timerLabel: UILabel!
    
    var timerSetterTimer: Timer? = Timer()
    
    override func viewWillAppear(_ animated: Bool) {
        
        timerSetter()
        
        titleField.text = selected.value(forKey: "title") as? String
        descriptionField.text = selected.value(forKey: "desc") as? String
        alertCheck.setOn(selected.value(forKey: "alert") as! Bool, animated: true)
        alertCheck.isUserInteractionEnabled = false     // Temporary
        completeCheck.setOn(selected.value(forKey: "complete") as! Bool, animated: true)
        alertDate.setDate(selected.value(forKey: "alert_date") as! Date, animated: true)
        
        if(alertCheck.isOn) {
            alertDate.isHidden = false
        } else {
            alertDate.isHidden = true
        }
    }
    
    @objc func timerSetter() {
        
        let time = selected.value(forKey: "start_date") as! Date
        var timer: Int = 0
        
        let start_time = Int(time.timeIntervalSince1970)
        let act_time = Int(Date().timeIntervalSince1970)
        
        timer = act_time - start_time
        
        timerLabel.text = String(format: "%.2d:%.2d:%.2d", timer/3600, (timer/60)%60, timer%60)
        
    }
    
    func scheduledTimerWithTimeInterval() {
        
        timerSetterTimer = Timer.scheduledTimer(timeInterval: 1.0, target: self, selector: #selector(EditViewController.timerSetter), userInfo: nil, repeats: true)
        
    }
    
    @IBAction func dateAppear(_ sender: Any) {
        
        if(alertCheck.isOn) {
            alertDate.isHidden = false
        } else {
            alertDate.isHidden = true
        }
        
    }
    
    @IBAction func editClick(_ sender: Any) {
        
        let context = returnContext()
        
        selected.setValue(titleField.text, forKey: "title")
        selected.setValue(descriptionField.text, forKey: "desc")
        
        // Predpriprava na alert
        selected.setValue(alertCheck.isOn, forKey: "alert")
        // TODO alertDate
        
        do
        {
            try context.save()
        }
        catch
        {
            // HANDLER
        }
        
        goals.removeAll()
        goals = loadCoreData()
        
        self.navigationController?.popViewController(animated: true)
    }
    
    @IBAction func checkComplete(_ sender: Any) {
        if(completeCheck.isOn) {
            let context = returnContext()
            
            selected.setValue(titleField.text, forKey: "title")
            selected.setValue(descriptionField.text, forKey: "desc")
            selected.setValue(alertCheck.isOn, forKey: "alert")
            selected.setValue(completeCheck.isOn,forKey: "complete")
            
            do
            {
                try context.save()
            }
            catch
            {
                // HANDLER
            }
            
            goals.removeAll()
            goals = loadCoreData()
            
            self.navigationController?.popViewController(animated: true)
        }
    }
    
    @IBAction func resetClick(_ sender: Any) {
        
        let context = returnContext()
        
        selected.setValue(Date(), forKey: "start_date")
        
        do
        {
            try context.save()
        }
        catch
        {
            // HANDLER
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.hideKeyboardWhenTappedAround()
        
        scheduledTimerWithTimeInterval()
        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        // Stoping timer
        timerSetterTimer?.invalidate()
        timerSetterTimer = nil
    }

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
