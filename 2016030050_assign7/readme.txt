Σχόλια για κώδικα-----------------------------------------------------------------------
	Το adblock.sh τρέχει κανονικά .Για την ολοκλήρωση του bash script 
	χρησιμοποιήθηκαν redirections , awk ,και pipelines πέραν του καθοριστικού για 
	τη λειτουργία iptables command.Τα rules ενημερώνονται κανονικά γίνονται 
	reset κ.ο.κ. σύμφωνα με τις προδιαγραφές.
	Επίσης στο παραδώτεο έχει ήδη εκτελεστεί η εντολή -domain η οποία πέρνει χρόνο
	(περίπου 8 λεπτά) έτσι ώστε να βοηθήσω οσο γίνετε με το γρηγορότερο για εσάς
	Testing του assign μου.
----------------------------------------------------------------------------------------

Απάντηση σε ερώτημα Εκφώνησης ----------------------------------------------------------
	Ο λόγος που δεν μπλοκάρονται κάποιες διαφημίσεις είναι :
	->Είτε δεν είναι περασμένα στη λίστα μας τα domain name και επομένως δεν 
		μπορούμε να τα αντιστοίχισουμε σε IPs και κατόπιν να τα μπλοκάρουμε .
	->Είτε επειδή οι διαφημιστές γνωρίζουν ότι εμείς μπορεί να χρησιμοποιούμε 
		adblocker αυτού του τύπου ,μπορεί να επιλέγουν σε συνεργασία με τα  site 
		που τους διαφημίζουν την συνεχή αλλαγή των IP ,δηλαδή την συνεχή 
		αλλαγή υπολογιστών που κάνουν Serve στα Request μας.
	
	Αυτό βεβαίως μπορεί να τύχει να είναι απλά αποτέλεσμα της αρχιτεκτονικής 
	του συστήματος . Οπως για παράδειγμα γίνετε μερικές φορές με Cloud Computing 
	παρόχους ,οι οποίοι ενδεχομένως αλλάζουν IP (Υπολογιστές εξυπηρέτησης) 
	μέσα στο χρόνο με αποτέλεσμα ένα domain να αντιστοιχεί σε περισσότερες της 1 IP.
----------------------------------------------------------------------------------------
